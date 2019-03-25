#include "pch.h"
#include "Window.h"

namespace Controls
{
	Window::Window(
		const std::wstring &title,
		const std::wstring &windowClass,
		int x,
		int y,
		int width,
		int height
	) : Control(windowClass, title, WS_OVERLAPPEDWINDOW, x, y, width, height)
	{
	}

	int Window::onResize(ResizeType type, const SIZE & size)
	{
		for (auto child : children)
		{
			int ret = child->onResize(type, size);
			if (ret) return ret;
		}

		return 0;
	}
}