#pragma once
#include "Direct2DCanvas.h"

namespace Controls
{
	class TerminalCanvas
		: public Direct2DCanvas
	{
	public:
		TerminalCanvas();

		void draw(wchar_t character);

		virtual void onRenderDxScene(ID2D1RenderTarget *target);
		virtual void onCreateDxResources(ID2D1RenderTarget *target);
		virtual void onReleaseDxResources();
		virtual int onResize(ResizeType type, const SIZE &size);

	private:
		ID2D1SolidColorBrush *pBrush;
		POINT cursorPos;
		void CalculateLayout();
	};
}
