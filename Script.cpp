#include <iostream>
#include <fstream>
#include "Script.hpp"
#include "PNG.hpp"
#include "XPM2.hpp"
#include <algorithm>

using namespace std;

namespace prog {
    // Use to read color values from a script file.
    istream& operator>>(istream& input, Color& c) {
        int r, g, b;
        input >> r >> g >> b;
        c.red() = r;
        c.green() = g;
        c.blue() = b;
        return input;
    }

    Script::Script(const string& filename) :
            image(nullptr), input(filename) {

    }
    void Script::clear_image_if_any() {
        if (image != nullptr) {
            delete image;
            image = nullptr;
        }
    }
    Script::~Script() {
        clear_image_if_any();
    }

    void Script::run() {
        string command;
        while (input >> command) {
            cout << "Executing command '" << command << "' ..." << endl;
            if (command == "open") {
                open();
                continue;
            }
            if (command == "blank") {
                blank();
                continue;
            }
            // Other commands require an image to be previously loaded.
            if (command == "save") {
                save();
                continue;
            } 
            if (command == "invert") {
                invert();
                continue;
            }
            if (command == "to_gray_scale") {
                to_gray_scale();
                continue;
            }
            if (command == "replace"){
                replace();
                continue;
            }
            if (command == "fill") {
                fill();
                continue;
            }
            if (command == "h_mirror") {
                h_mirror();
                continue;
            }
            if (command == "v_mirror") {
                v_mirror();
                continue;
            }
            if (command == "add") {
                add();
                continue;
            }
            if (command == "crop") {
                crop();
                continue;
            }
            if (command == "rotate_left") {
                rotate_left();
                continue;
            }
            if (command == "rotate_right") {
                rotate_right();
                continue;
            }
            if (command == "median_filter") {
                median_filter();
                continue;
            }
            if (command == "xpm2_open") {
                clear_image_if_any();
                std::string file;
                input >> file;
                image = loadFromXPM2(file);
                continue;
            }
            if (command == "xpm2_save") {
                std::string file;
                input >> file;
                saveToXPM2(file, image);
                continue;
            }
        }
    }
    void Script::open() {
        // Replace current image (if any) with image read from PNG file.
        clear_image_if_any();
        string filename;
        input >> filename;
        image = loadFromPNG(filename);
    }
    void Script::blank() {
        // Replace current image ./tes(if any) with blank image.
        clear_image_if_any();
        int w, h;
        Color fill;
        input >> w >> h >> fill;
        image = new Image(w, h, fill);
    }
    void Script::save() {
        // Save current image to PNG file.
        string filename;
        input >> filename;
        saveToPNG(filename, image);
    }

    void Script::invert() {
        // Transforms each individual pixel (red, green, blue) to (255-red, 255-green, 255-blue)
        for(int row = 0; row < image->height(); row++){
            for(int col = 0; col < image->width(); col++){
                image->at(col, row).red() = 255-image->at(col,row).red();
                image->at(col, row).green() = 255-image->at(col, row).green();
                image->at(col, row).blue() = 255-image->at(col, row).blue();
            }
        }
    }

    void Script::to_gray_scale() {
        // Transforms each individual pixel (red, green, blue) to (v, v, v) where v = (red + green + blue)/3
        for(int row = 0; row < image->height(); row++){
            for(int col = 0; col < image->width(); col++){
                unsigned int r=image->at(col,row).red();
                unsigned int g=image->at(col,row).green();
                unsigned int b=image->at(col,row).blue();
                image->at(col,row).red()=(r+g+b)/3;
                image->at(col,row).green()=(r+g+b)/3;
                image->at(col,row).blue()=(r+g+b)/3;
            }
        }
    }

    void Script::replace() {
        // Replaces all (r1, g1, b1) pixels by (r2, g2, b2)
        unsigned int r1, g1, b1, r2, g2, b2, r, g, b;
        input >> r1 >> g1 >> b1 >> r2 >> g2 >> b2;
        for(int row = 0; row < image->height(); row++){
            for(int col = 0; col < image->width(); col++){
                r=image->at(col,row).red();
                g=image->at(col,row).green();
                b=image->at(col,row).blue();
                if(r==r1 && g==g1 && b==b1){
                    image->at(col,row).red()=r2;
                    image->at(col,row).green()=g2; 
                    image->at(col,row).blue()=b2;
                }
            }
        }
    }   

    void Script::fill() {
        // Assign (r, g, b) to all pixels contained in rectangle defined by top-left corner (x, y) width w and height h
        int x, y, w, h;
        unsigned int r, g, b;
        input >> x >> y >> w >> h >> r >> g >> b;
        for(int row = y; row < h+y; row++){
            for(int col = x; col < w+x; col++){
                image->at(col, row).red() = r;
                image->at(col, row).green() = g;
                image->at(col, row).blue() = b;
            }
        }
    }

    void Script::h_mirror() {
        // Mirror image horizontally
        for (int row = 0; row < image->height(); row++) {
            int middle_col = image->width() / 2;    // Index of the middle column
            for (int col = 0; col < middle_col; col++) {   // Iterate until middle column
                std::swap(image->at(col, row), image->at(image->width() - 1 - col, row));   // swap the first pixel with the last pixel of the same line until reach the middle column 
            }
        }
    }

    void Script::v_mirror() {
        // Mirror image vertically
        for (int col = 0; col < image->width(); col++) {
            int middle_row = image->height() / 2;   // Index of the middle row
            for (int row = 0; row < middle_row; row++) {    // Iterate until middle row
                std::swap(image->at(col, row), image->at(col, image->height() - 1 - row));  // swap the first pixel with the last pixel of the same column until reach the middle row
            }
        }
    }

    void Script::add() {    
        /* 
        Copy all pixels from images stored in PNG file except pixels in that 
        image with "neutral" color (r, g, b), to the rectangle of the current 
        image with top-left corner (x, y) of the current image  
        */
        string filename;
        unsigned int r, g, b;
        int x, y;
        input >> filename >> r >> g >> b >> x >> y;
        Image* new_image = loadFromPNG(filename);   
        for (int col = 0; col < new_image->width(); col++){
            for (int row = 0; row < new_image->height(); row++){
                if(new_image->at(col,row).red() != r || new_image->at(col,row).green() != g || new_image->at(col,row).blue() != b){
                    // If new_image pixel color is different from (r, g, b) change image pixel
                    image->at(col+x,row+y).red() = new_image->at(col,row).red();
                    image->at(col+x,row+y).blue() = new_image->at(col,row).blue();
                    image->at(col+x,row+y).green() = new_image->at(col,row).green();
                }
            }
        }
        delete new_image;
    }

    void Script::crop() {
        // Crop the image, reducing it to all pixels contained in the rectangle defined by top-left corner (x, y), width w, and height h
        int x, y, w, h;
        input >> x >> y >> w >> h;
        Image *cropped_image = new Image(w, h); // Image where we save the cropped image
        for (int row = y; row < y+h; row++) {
            for(int col = x; col < x+w; col++) {
                cropped_image->at(col-x, row-y) = image->at(col, row);  // The top-left corner of the cropped image (0, 0) will be the the top-left corner (x, y) of the original image
            }
        }
        clear_image_if_any();
        image = cropped_image;
    }

    void Script::rotate_left() {
        // Rotate image left by 90 degrees
        Image *rotate90 = new Image(image->height(), image->width());   // Image where we save the result
        for (int row = 0; row < image->height(); row++) {
            for (int col = 0; col < image->width(); col++) {
                rotate90->at(row, col) = image->at(image->width()-col-1, row);  // The first line of the result image will be the last corner of the original image
            }
        }
        clear_image_if_any();
        image = rotate90;
    }

    void Script::rotate_right() {
        // Rotate image right by 90 degrees
        Image *rotate90 = new Image(image->height(), image->width());   // Image where we save the result
        for (int row = 0; row < image->height(); row++) {
            for (int col = 0; col < image->width(); col++) {
                rotate90->at(row, col) = image->at(col, image->height()-row-1); // The first line of the result image will be the inverse of the first column of the original image
            }
        }
        clear_image_if_any();
        image = rotate90;
    }
    
    void find_neighbourhoud(const Image* image, int ws, int row, int col, std::vector<Color> &median) {
        /* 
        Find the adjacent pixels of the middle pixel with a radius of ws/2
        and append them to the vector "median"
        */
        int aux = ws / 2;
        int min_row = 0, min_col = 0;
        int max_row = image->height() - 1, max_col = image->width() - 1;
        if (row - aux >= 0)
            min_row = row - aux;
        if (col - aux >= 0)
            min_col = col - aux;
        if (row + aux <= max_row)
            max_row = row + aux;
        if (col + aux <= max_col)
            max_col = col + aux;
        
        for (int r = min_row; r <= max_row; r++) {
            for (int c = min_col; c <= max_col; c++) {
                median.push_back(image->at(c, r));
            }
        }
    }
    
    Color calculate_median(const std::vector<Color>& median) {
        /*
        Calculate the median Color of pixels in the median vector sort each
        RGB Color and finding the median value of each one and then return
        the median Color 
        */
        Color res;
        std::vector<int> reds, greens, blues;
        for (const Color& aux : median) {
            reds.push_back(aux.red());
            greens.push_back(aux.green());
            blues.push_back(aux.blue());
        }
        std::sort(reds.begin(), reds.end());
        std::sort(greens.begin(), greens.end());
        std::sort(blues.begin(), blues.end());
        
        if (median.size() % 2 == 0) {
            res.red() = ((reds[reds.size() / 2] + reds[(reds.size() / 2 - 1)]) / 2);
            res.green() = ((greens[greens.size() / 2] + greens[(greens.size() / 2 - 1)]) / 2);
            res.blue() = ((blues[blues.size() / 2] + blues[(blues.size() / 2 - 1)]) / 2);
        }
        else{
            res.red() = reds[reds.size() / 2];
            res.green() = greens[greens.size() / 2];
            res.blue() = blues[blues.size() / 2];
        }
        return res;
    }

    void Script::median_filter() {
        // Apply a median filter with window size ws to the current image
        int ws;
        input >> ws;
        Image *modified = new Image(image->width(), image->height());
        for (int row = 0; row < image->height(); row++) {
            for (int col = 0; col < image->width(); col++) {
                std::vector<Color> median;  // vector to place the adjecent pixels of the median pixel
                find_neighbourhoud(image, ws, row, col, median);    // auxiliary function
                modified->at(col, row) = calculate_median(median);  // auxiliary function
            }
        }
        clear_image_if_any();
        image = modified;
    }
}
