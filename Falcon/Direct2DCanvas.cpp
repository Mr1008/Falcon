#include "pch.h"
#include "Direct2DCanvas.h"

namespace Controls
{
	template <class T>
	void SafeRelease(T **ppT)
	{
		if (*ppT)
		{
			(*ppT)->Release();
			*ppT = NULL;
		}
	}

	Direct2DCanvas::Direct2DCanvas(const std::wstring &className, int x, int y, int width, int height)
		: Control(className, L"", WS_CHILD | WS_VISIBLE, x, y, width, height)
	{
		hasCustomPaint = true;
	}

	int Direct2DCanvas::onCreate()
	{
		if (FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pFactory)))
		{
			return -1;
		}
		return 0;
	}

	int Direct2DCanvas::onPaint(HDC hdi, PAINTSTRUCT *ps)
	{
		HRESULT hr = CreateGraphicsResources(&ps->rcPaint);
		if (SUCCEEDED(hr))
		{
			pRenderTarget->BeginDraw();
			pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::SkyBlue));
			pRenderTarget->FillEllipse(ellipse, pBrush);

			hr = pRenderTarget->EndDraw();
			if (FAILED(hr) || hr == D2DERR_RECREATE_TARGET)
			{
				DiscardGraphicsResources();
			}
		}
		return 0;
	}

	int Direct2DCanvas::onDestroy()
	{
		DiscardGraphicsResources();
		SafeRelease(&pFactory);
		PostQuitMessage(0);
		return 0;
	}

	int Direct2DCanvas::onResize(ResizeType type, const SIZE &size)
	{
		setControlPosAndSize(nullptr, &size);
		if (pRenderTarget)
		{
			D2D1_SIZE_U d2size = D2D1::SizeU(size.cx, size.cy);
			pRenderTarget->Resize(d2size);
			CalculateLayout();
			InvalidateRect(getHwnd(), nullptr, false);
		}
		return 0;
	}

	void Direct2DCanvas::DiscardGraphicsResources()
	{
		SafeRelease(&pRenderTarget);
		SafeRelease(&pBrush);
	}

	void Direct2DCanvas::CalculateLayout()
	{
		if (pRenderTarget != NULL)
		{
			D2D1_SIZE_F size = pRenderTarget->GetSize();
			const float x = size.width / 2;
			const float y = size.height / 2;
			const float radius = min(x, y);
			ellipse = D2D1::Ellipse(D2D1::Point2F(x, y), radius, radius);
		}
	}

	HRESULT Direct2DCanvas::CreateGraphicsResources(RECT *rect)
	{
		HRESULT hr = S_OK;
		if (pRenderTarget == NULL)
		{
			D2D1_SIZE_U size = D2D1::SizeU(rect->right, rect->bottom);

			hr = pFactory->CreateHwndRenderTarget(
				D2D1::RenderTargetProperties(),
				D2D1::HwndRenderTargetProperties(getHwnd(), size),
				&pRenderTarget);

			if (SUCCEEDED(hr))
			{
				const D2D1_COLOR_F color = D2D1::ColorF(1.0f, 1.0f, 0);
				hr = pRenderTarget->CreateSolidColorBrush(color, &pBrush);

				if (SUCCEEDED(hr))
				{
					CalculateLayout();
				}
			}
		}
		return hr;
	}
}