#pragma once

namespace Engine
{
    struct Color
    {
        Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) : r(r), g(g), b(b), a(a)
        {}

        uint8_t r;
        uint8_t g;
        uint8_t b;
        uint8_t a;
    };

    enum ColorReference
    {
        Default,
        Black,
        Red,
        Green,
        Yellow,
        Blue,
        Magenta,
        Cyan,
        White,
        BrightBlack,
        BrightRed,
        BrightGreen,
        BrightYellow,
        BrightBlue,
        BrightMagenta,
        BrightCyan,
        BrightWhite
    };

    class ColorMapper
    {
    public:
        Color mapReferenceToColor(ColorReference reference, const Color&& defaultColor) const;
    };
}