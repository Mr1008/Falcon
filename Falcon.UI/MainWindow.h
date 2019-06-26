#pragma once
#include "DirectXWindow.h"

namespace Controls
{
    class MainWindow :
        public DirectXWindow
    {
    public:
        MainWindow(
            const std::wstring& title,
            int x = CW_USEDEFAULT,
            int y = CW_USEDEFAULT,
            int width = CW_USEDEFAULT,
            int height = CW_USEDEFAULT
        ) : DirectXWindow(title, L"MainWindowViewClass", x, y, width, height)
        {
        }

        virtual void onDestroy();
    };
}
