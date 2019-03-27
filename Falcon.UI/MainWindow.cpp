#include "pch.h"
#include "MainWindow.h"

namespace Controls
{
	int MainWindow::onPaint(HDC hdc, PAINTSTRUCT *ps)
	{
		FillRect(hdc, &ps->rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
		return 0;
	}

	int MainWindow::onDestroy()
	{
		PostQuitMessage(0);
		return 0;
	}
}