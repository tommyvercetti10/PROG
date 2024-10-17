#ifndef __prog_Color_hpp__
#define __prog_Color_hpp__

namespace prog
{
    typedef unsigned char rgb_value;
    class Color
    {
    private: 
        rgb_value r_;
        rgb_value g_;
        rgb_value b_;

    public:
        Color();
        Color(const Color& c);
        Color(rgb_value red,rgb_value green, rgb_value blue);
        bool operator<(const Color& rgb) const;
        rgb_value red() const;
        rgb_value& red();
        rgb_value green() const;
        rgb_value& green();
        rgb_value blue() const;
        rgb_value& blue();
    };
}
#endif
