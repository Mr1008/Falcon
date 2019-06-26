#pragma once

namespace Engine
{
    struct Color
    {
        Color(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b)
        {}

        uint8_t r;
        uint8_t g;
        uint8_t b;
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
}