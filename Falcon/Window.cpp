#include "pch.h"
#include "Window.h"

using namespace Views;

void Window::onPaint(HDC hdc, PAINTSTRUCT *ps)
{
	FillRect(hdc, &ps->rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
}
