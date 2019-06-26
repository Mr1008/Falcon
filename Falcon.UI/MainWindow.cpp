#include "pch.h"
#include "MainWindow.h"

namespace Controls
{
    void MainWindow::onDestroy()
    {
        DirectXWindow::onDestroy();
        PostQuitMessage(0);
    }
}