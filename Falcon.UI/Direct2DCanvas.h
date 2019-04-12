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
			const std::wstring &className);

		virtual int onCreate();
		virtual void onCreated();
		virtual int onPaint(HDC hdi, PAINTSTRUCT *ps);
		virtual int onDestroy();
		virtual int onResize(ResizeType type, const SIZE &size);
		virtual void onRenderDxScene(ID2D1RenderTarget *target) = 0;
		virtual void onReleaseDxResources();
		virtual void onCreateDxResources(ID2D1RenderTarget *target) = 0;

	protected:
		template <class T>
		void releaseDxResource(T **ppT);

	private:
		ID2D1Factory *factory;
		ID2D1HwndRenderTarget *renderTarget;

		HRESULT createGraphicsResources(RECT *rect);
	};
}
