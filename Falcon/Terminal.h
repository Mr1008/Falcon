#pragma once
#include "Direct2DCanvas.h"

namespace Controls
{
	class Terminal
		: public Direct2DCanvas
	{
	public:
		Terminal();

		virtual void onRenderDxScene(ID2D1RenderTarget *target);
		virtual void onCreateDxResources(ID2D1RenderTarget *target);
		virtual void onReleaseDxResources();
		virtual int onResize(ResizeType type, const SIZE &size);

	private:
		ID2D1SolidColorBrush *pBrush;
		D2D1_ELLIPSE ellipse;

		void CalculateLayout();
	};
}
