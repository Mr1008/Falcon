#include "pch.h"
#include "MainWindow.h"

namespace Controls
{
	int MainWindow::onDestroy()
	{
		DirectXWindow::onDestroy();
		PostQuitMessage(0);
		return 0;
	}
}