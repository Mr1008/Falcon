#include "pch.h"
#include "Direct2DCanvas.h"

namespace Controls
{
	using namespace std;

	Direct2DCanvas::Direct2DCanvas(const std::wstring &className) :
		Control(className, L"", WS_CHILD | WS_VISIBLE, 0, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT),
		factory(nullptr),
		renderTarget(nullptr)
	{
		hasCustomPaint = true;
	}

	template <class T>
	void Direct2DCanvas::releaseDxResource(T** ppT)
	{
		if (*ppT)
		{
			(*ppT)->Release();
			*ppT = nullptr;
		}
	}

	int Direct2DCanvas::onCreate()
	{
		if (FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &factory)))
		{
			return -1;
		}
		return Control::onCreate();
	}

	void Direct2DCanvas::onCreated() 
	{
		Control::onCreated();
	}

	int Direct2DCanvas::onPaint(HDC hdi, PAINTSTRUCT *ps)
	{
		HRESULT hr = createGraphicsResources(&ps->rcPaint);
		if (SUCCEEDED(hr))
		{
			renderTarget->BeginDraw();
			onRenderDxScene(renderTarget);
			hr = renderTarget->EndDraw();
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
		if (renderTarget)
		{
			D2D1_SIZE_U d2size = D2D1::SizeU(size.cx, size.cy);
			renderTarget->Resize(d2size);
			forceRender();
		}
		return 0;
	}

	void Direct2DCanvas::onReleaseDxResources()
	{
		releaseDxResource(&renderTarget);
		releaseDxResource(&factory);
	}


	HRESULT Direct2DCanvas::createGraphicsResources(RECT *rect)
	{
		HRESULT hr = S_OK;
		if (renderTarget == nullptr)
		{
			D2D1_SIZE_U size = D2D1::SizeU(rect->right, rect->bottom);

			hr = factory->CreateHwndRenderTarget(
				D2D1::RenderTargetProperties(),
				D2D1::HwndRenderTargetProperties(getHwnd(), size),
				&renderTarget);

			if (SUCCEEDED(hr))
			{
				onCreateDxResources(renderTarget);
			}
		}
		return hr;
	}
}