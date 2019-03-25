#include "pch.h"
#include "Terminal.h"

namespace Controls 
{
	Terminal::Terminal()
		: Direct2DCanvas(L"HwAcceleratedTerminal")
	{
	}
	void Terminal::onRenderDxScene(ID2D1RenderTarget *target)
	{
		target->Clear(D2D1::ColorF(D2D1::ColorF::SkyBlue));
		target->FillEllipse(ellipse, pBrush);
	}

	void Terminal::onCreateDxResources(ID2D1RenderTarget * target)
	{
		const D2D1_COLOR_F color = D2D1::ColorF(1.0f, 1.0f, 0);
		HRESULT hr = target->CreateSolidColorBrush(color, &pBrush);

		if (SUCCEEDED(hr))
		{
			CalculateLayout();
		}
	}

	void Terminal::onReleaseDxResources()
	{
		pBrush->Release();
		pBrush = nullptr;
	}

	int Terminal::onResize(ResizeType type, const SIZE &size)
	{
		int ret = Direct2DCanvas::onResize(type, size);
		if (ret) return ret;

		CalculateLayout();

		return 0;
	}

	void Terminal::CalculateLayout()
	{
		SIZE size = getSize();
		const float x = size.cx / 2.f;
		const float y = size.cy / 2.f;
		const float radius = min(x, y);
		ellipse = D2D1::Ellipse(D2D1::Point2F(x, y), radius, radius);
	}
}