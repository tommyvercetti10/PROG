#include "Image.hpp"

namespace prog
{
    Image::Image(int w, int h, const Color& fill)
    {
        // Constructor to create a image
        width_ = w;
        height_ = h;
        pixels = std::vector<std::vector<Color>>(h, std::vector<Color>(w, fill));
    }

    Image::~Image()
    {
        // Destructor (not used)
    }

    int Image::width() const
    {
        // Get image width
        return width_;
    }

    int Image::height() const
    {
        // Get image height
        return height_;
    }

    Color& Image::at(int x, int y) 
    {
        // Get mutable reference to the value of a pixel
        return pixels[y][x];
    }

    const Color& Image::at(int x, int y) const  
    {
        // Get read-only reference to the value of a pixel
        return pixels[y][x];
    }
}