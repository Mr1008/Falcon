#pragma once

namespace Engine
{
    struct Color
    {
        Color(char r, char g, char b) : r(r), g(g), b(b)
        {}

        char r;
        char g;
        char b;
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