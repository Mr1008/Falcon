#pragma once
#include "ViewBase.h"

namespace Views
{
	class Window
		: public ViewBase
	{
	public:
		Window(
			const std::wstring &title,
			const std::wstring &windowClass,
			int x = CW_USEDEFAULT,
			int y = CW_USEDEFAULT,
			int width = CW_USEDEFAULT,
			int height = CW_USEDEFAULT
		) : ViewBase(windowClass, title, WS_OVERLAPPEDWINDOW, x, y, width, height)
		{
		}
	};
}

