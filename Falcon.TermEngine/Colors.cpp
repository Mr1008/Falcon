#include "pch.h"
#include "Colors.h"

namespace Engine
{
    Color ColorMapper::mapReferenceToColor(ColorReference reference, const Color&& defaultColor) const
    {
        switch (reference) {
        case ColorReference::Default:
            return defaultColor;
        case ColorReference::Black:
            return Color(1, 1, 1);
        case ColorReference::Red:
            return Color(222, 56, 43);
        case ColorReference::Green:
            return Color(57, 181, 74);
        case ColorReference::Yellow:
            return Color(255, 199, 6);
        case ColorReference::Blue:
            return Color(0, 111, 184);
        case ColorReference::Magenta:
            return Color(118, 38, 113);
        case ColorReference::Cyan:
            return Color(44, 181, 233);
        case ColorReference::White:
            return Color(204, 204, 204);
        case ColorReference::BrightBlack:
            return Color(128, 128, 128);
        case ColorReference::BrightRed:
            return Color(255, 0, 0);
        case ColorReference::BrightGreen:
            return Color(0, 255, 0);
        case ColorReference::BrightYellow:
            return Color(255, 255, 0);
        case ColorReference::BrightBlue:
            return Color(0, 0, 255);
        case ColorReference::BrightMagenta:
            return Color(255, 0, 255);
        case ColorReference::BrightCyan:
            return Color(0, 255, 255);
        case ColorReference::BrightWhite:
            return Color(255, 255, 255);
        }
    }
}
