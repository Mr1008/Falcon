#pragma once
#include "ViewBase.h"
#include <d2d1.h>
#pragma comment(lib, "d2d1")

namespace Views {
	class Direct2DCanvas :
		public ViewBase
	{
	public:
		Direct2DCanvas(
			const std::wstring &className,
			int x = CW_USEDEFAULT,
			int y = CW_USEDEFAULT,
			int width = CW_USEDEFAULT,
			int height = CW_USEDEFAULT);

	protected:
		virtual void onCreate();
		virtual void onPaint(HDC hdi, PAINTSTRUCT *ps);
		virtual void onDestroy();

	private:
		ID2D1Factory            *pFactory;
		ID2D1HwndRenderTarget   *pRenderTarget;
	};
}
