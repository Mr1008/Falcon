#include "pch.h"
#include "MainWindow.h"

using namespace Views;

void MainWindow::onPaint(HDC hdc, PAINTSTRUCT *ps)
{
	FillRect(hdc, &ps->rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
}
