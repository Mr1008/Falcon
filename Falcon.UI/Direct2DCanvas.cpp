#include "pch.h"
#include "Direct2DCanvas.h"

namespace Controls
{
	using namespace std;

	template <class T>
	void Direct2DCanvas::releaseDxResource(T **ppT)
	{
		if (*ppT)
		{
			(*ppT)->Release();
			*ppT = nullptr;
		}
	}

	Direct2DCanvas::Direct2DCanvas(const std::wstring &className)
		: Control(className, L"", WS_CHILD | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT)
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
		HRESULT hr = createGraphicsResources(&ps->rcPaint);
		if (SUCCEEDED(hr))
		{
			pRenderTarget->BeginDraw();
			onRenderDxScene(pRenderTarget);
			hr = pRenderTarget->EndDraw();
			if (FAILED(hr) || hr == D2DERR_RECREATE_TARGET)
			{
				onReleaseDxResources();
			}
		}
		return 0;
	}

	int Direct2DCanvas::onDestroy()
	{
		onReleaseDxResources();
		return 0;
	}

	int Direct2DCanvas::onResize(ResizeType type, const SIZE &size)
	{
		setControlPosAndSize(nullptr, &size);
		if (pRenderTarget)
		{
			D2D1_SIZE_U d2size = D2D1::SizeU(size.cx, size.cy);
			pRenderTarget->Resize(d2size);
			forceRender();
		}
		return 0;
	}

	void Direct2DCanvas::onReleaseDxResources()
	{
		releaseDxResource(&pRenderTarget);
		releaseDxResource(&pFactory);
	}


	HRESULT Direct2DCanvas::createGraphicsResources(RECT *rect)
	{
		HRESULT hr = S_OK;
		if (pRenderTarget == nullptr)
		{
			D2D1_SIZE_U size = D2D1::SizeU(rect->right, rect->bottom);

			hr = pFactory->CreateHwndRenderTarget(
				D2D1::RenderTargetProperties(),
				D2D1::HwndRenderTargetProperties(getHwnd(), size),
				&pRenderTarget);

			if (SUCCEEDED(hr))
			{
				onCreateDxResources(pRenderTarget);
			}
		}
		return hr;
	}
}