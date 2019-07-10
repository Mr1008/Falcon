#pragma once
#include "pch.h"

namespace Engine
{
    class TerminalWindowActions
    {
    public:
        virtual void setTitle(const std::wstring& title) = 0;
    };
}