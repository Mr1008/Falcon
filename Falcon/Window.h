#pragma once
#include "ViewBase.h"
#include <string>

namespace Views
{
	class Window
		: public ViewBase
	{
	public:
		Window(
			const ViewBase* parent,
			std::wstring title,
			std::wstring windowClass,
			int x = CW_USEDEFAULT,
			int y = CW_USEDEFAULT,
			int width = CW_USEDEFAULT,
			int height = CW_USEDEFAULT
		) : ViewBase(windowClass, parent, title, WS_OVERLAPPEDWINDOW, x, y, width, height)
		{
		}

	protected:
		virtual void onPaint(HDC hdc, PAINTSTRUCT *ps);
	};
}

