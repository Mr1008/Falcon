#pragma once
#include "Control.h"

namespace Controls
{
	class Window :
		public Control
	{
	public:
		Window(
			const std::wstring &title,
			const std::wstring &windowClass,
			int x = CW_USEDEFAULT,
			int y = CW_USEDEFAULT,
			int width = CW_USEDEFAULT,
			int height = CW_USEDEFAULT
		);

		virtual int onResize(ResizeType type, const SIZE &size);
	};
}
