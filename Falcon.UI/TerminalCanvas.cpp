#include "pch.h"
#include "TerminalCanvas.h"

namespace Controls
{
	TerminalCanvas::TerminalCanvas()
		: Direct2DCanvas(L"HwAcceleratedTerminal")
	{
	}

	void TerminalCanvas::draw(wchar_t character)
	{

	}

	void TerminalCanvas::onRenderDxScene(ID2D1RenderTarget *target)
	{
		target->Clear(D2D1::ColorF(D2D1::ColorF::Black));
	}

	void TerminalCanvas::onCreateDxResources(ID2D1RenderTarget * target)
	{
		const D2D1_COLOR_F color = D2D1::ColorF(1.0f, 1.0f, 1.0f);
		HRESULT hr = target->CreateSolidColorBrush(color, &pBrush);

		if (SUCCEEDED(hr))
		{
			CalculateLayout();
		}
	}

	void TerminalCanvas::onReleaseDxResources()
	{
		pBrush->Release();
		pBrush = nullptr;
	}

	int TerminalCanvas::onResize(ResizeType type, const SIZE &size)
	{
		int ret = Direct2DCanvas::onResize(type, size);
		if (ret) return ret;

		CalculateLayout();

		return 0;
	}

	void TerminalCanvas::CalculateLayout()
	{
	}
}