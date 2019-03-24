#include "pch.h"
#include "Direct2DCanvas.h"

using namespace Views;

Direct2DCanvas::Direct2DCanvas(const std::wstring & className, int x, int y, int width, int height)
	: ViewBase(className, L"", WS_CHILD | WS_VISIBLE, x, y, width, height)
{
	hasCustomPaint = true;
}

void Direct2DCanvas::onCreate()
{
}

void Direct2DCanvas::onPaint(HDC hdi, PAINTSTRUCT * ps)
{
}

void Direct2DCanvas::onDestroy()
{
}
