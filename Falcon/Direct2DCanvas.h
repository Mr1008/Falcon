#pragma once
#include "Control.h"
#include <d2d1.h>
#pragma comment(lib, "d2d1")

namespace Controls {
	class Direct2DCanvas :
		public Control
	{
	public:
		Direct2DCanvas(
			const std::wstring &className,
			int x = CW_USEDEFAULT,
			int y = CW_USEDEFAULT,
			int width = CW_USEDEFAULT,
			int height = CW_USEDEFAULT);

	protected:
		virtual int onCreate();
		virtual int onPaint(HDC hdi, PAINTSTRUCT *ps);
		virtual int onDestroy();
		virtual int onResize(ResizeType type, const SIZE &size);

	private:
		ID2D1Factory            *pFactory;
		ID2D1HwndRenderTarget   *pRenderTarget;
		ID2D1SolidColorBrush    *pBrush;
		D2D1_ELLIPSE            ellipse;

		void DiscardGraphicsResources();
		void CalculateLayout();
		HRESULT CreateGraphicsResources(RECT *rect);
	};
}
