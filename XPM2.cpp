#include "XPM2.hpp"
#include "sstream"
#include "fstream"
#include "map"
using namespace std;

namespace prog {

    int hex_to_decimal(const std::string& str) { 
        /*
        Function that turns a string of two characters (that 
        represents a hexadecimal value) into a corresponding 
        integer that will be used to create a corresponding Color
        */
        int res = 0;
        int x = 0;
        for (int i = 0; i < 2; i++) {
            if (str[i] >= '0' && str[i] <= '9') {
                x = str[i] - '0';
            }
            else if (str[i] >= 'a' && str[i] <= 'f') {
                x = str[i] - 87;
            }
            else if (str[i] >= 'A' && str[i] <= 'F') {
                x = str[i] - 55;
            }
            res = res*16 + x;
        }
        return res;
    }

    Color hex_to_color(const std::string& str) {
        // Function that receives a string in '#XXXXXX' format and returns a Color
        Color res;
        std::string str_;
        if (str[0] == '#')
            str_ = str.substr(1, 6);

        std::string r = str_.substr(0, 2);
        std::string g = str_.substr(2, 2);
        std::string b = str_.substr(4, 2);
        res.red() = hex_to_decimal(r);
        res.blue() = hex_to_decimal(b);
        res.green() = hex_to_decimal(g);
        return res;
    }


    Image* loadFromXPM2(const std::string& file) {
        // Function that read an image stored in the XPM2 file format
        ifstream in(file);
        string line, caracter, str, useless;
        map<char, Color> mapa;
        int png_height, png_width, colors, chars;
        getline(in,line); //Reads the first line and ignores it
        getline(in,line); //Reads the second line
        istringstream aux(line); //Breaks the second line into words
        aux >> png_width >> png_height >> colors >> chars; //Saves the info in second line

        // For cicle to save the colors with its corresponding char in a map
        for(int i=0; i<colors; i++){ 
            getline(in, line);
            istringstream aux(line);
            aux >> caracter >> useless >> str;
            mapa[caracter.at(0)] = hex_to_color(str);
        }
        Image* image = new Image(png_width, png_height);

        // For cicles to create the image in PNG format
        for(int row = 0; row < png_height; row++){ 
            getline(in, line);
            for(int col = 0; col < png_width; col++){
                image->at(col, row) = mapa[line.at(col)];
            }
        }
        return image;
    }

    string int_to_hex(const int& value) {
        /*
        Function that turns an integer into a corresponding string with two characters 
        that represent a hexadecimal value and will be used to create a corresponding Color      
        */
        std::string res = "";
        char ch;
        int aux, value_ = value;
        for (int i = 0; i < 2; i++) {
            aux = value_ % 16;
            if (aux >= 0 && aux <= 9) {
                ch = aux + '0';
            }
            else if (aux >= 10 && aux <= 15) {
                aux -= 10;
                ch = aux + 'a';
                
            }
            res.push_back(ch);
            value_ /= 16;
        }
        swap(res[0], res[1]);
        return res;
    }

    string color_to_hex(const Color& color) {
        // Function that returns a string in '#XXXXXX'format corresponding to a Color
        int r = color.red(), g = color.green(), b = color.blue();
        std::string res;
        res.append("#");
        res.append(int_to_hex(r));
        res.append(int_to_hex(g));
        res.append(int_to_hex(b));
        return res;
    }

    void saveToXPM2(const std::string& file, const Image* image) {
        // Save image to the XPM2 file format
        ofstream out(file);
        out << "! XPM2" << '\n'; //First line
        int xpm2_height, xpm2_width, colors = 0;
        xpm2_height = image->height();
        xpm2_width = image->width();
        map<Color, char> mapa;

        // For cicles to save the number of colors
        for(int row = 0; row<xpm2_height; row++) {
            for(int col = 0; col<xpm2_width; col++) {
                if(mapa.count(image->at(col, row))==0) {
                    mapa[image->at(col, row)] = 'a' + colors;
                    colors++;
                }
            }
        }
        out << xpm2_width << " " << xpm2_height << " " << colors << " 1" << '\n'; //Second line

        // For cicle to create the XPM2 colors section
        for (const auto& pair : mapa) {
            out << pair.second << " c " << color_to_hex(pair.first) << '\n';
        }

        // For cicle to create the image in XPM2 format
        for(int row = 0; row<xpm2_height; row++){
            for(int col = 0; col<xpm2_width; col++){
                out << mapa[image->at(col, row)];
            }
            out << '\n';
        }
    }
}

