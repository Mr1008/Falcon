#pragma once
#include "Window.h"

namespace Views
{
	class MainWindow :
		public virtual Window
	{
	public:
		MainWindow(
			LPCWSTR title,
			int x = CW_USEDEFAULT,
			int y = CW_USEDEFAULT,
			int width = CW_USEDEFAULT,
			int height = CW_USEDEFAULT
		): Window(nullptr, title, L"MainWindowViewClass", x, y, width, height) 
		{
		}
	};
}
