#include "Color.hpp"

namespace prog {
    Color::Color() {
        // Default constructor
        r_ = 0;
        g_ = 0;
        b_ = 0;
    }
    Color::Color(const Color& c) {
        // Copy constructor
        r_ = c.red();
        g_ = c.green();
        b_ = c.blue();
    }
    Color::Color(rgb_value red, rgb_value green, rgb_value blue) {
        // Constructor using suplied (red, green, blue) values
        r_ = red;
        g_ = green;
        b_ = blue;
    }
    bool Color::operator<(const Color& rgb) const{
        // Constructor to use in any map with Color keys
        if (r_ < rgb.r_)
            return true;
        if (r_ == rgb.r_ && g_ < rgb.g_)
            return true;
        if (r_ == rgb.r_ && g_ == rgb.g_ && b_ < rgb.b_)
            return true;
        return false;
    }

    // Get values for individual RGB color channels
    rgb_value Color::red() const {
        return r_;
    }
    rgb_value Color::green() const {
        return g_;
    }
    rgb_value Color::blue() const {
        return b_;
    }

    // Get (mutable) references for individual RGB color channels
    rgb_value& Color::red()  {
        return r_;
    }
    rgb_value& Color::green()  {
        return g_;
    }
    rgb_value& Color::blue()  {
        return b_;
    }
}

