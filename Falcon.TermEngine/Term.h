#pragma once
#include <vector>
#include <dwrite.h>
#include "../Falcon.UI/TerminalCanvas.h"
#include "../Falcon.UI/InputListener.h"
#include "../Falcon.UI/Pointer.h"

namespace Engine
{
	class Term :
		public Controls::InputListener,
		public Controls::TerminalRenderer
	{
	public:
		Term(Controls::TerminalCanvas *canvas);

		virtual void onMouseMoved(const POINT &pos);
		virtual void onKeyPushed(wchar_t key, bool isFirstOccurence, unsigned int repeatCount);
		virtual void onMouseButtonDown(const POINT &pos, Controls::MouseButton button);
		virtual void onMouseButtonUp(const POINT &pos, Controls::MouseButton button);
		virtual void onRenderDxScene(ID2D1RenderTarget *target);
		virtual void onCreateDxResources(ID2D1RenderTarget *target);
		virtual void onReleaseDxResources();
		virtual int onResizeScene(Controls::ResizeType type, const SIZE &size);

	private:
		Controls::TerminalCanvas *canvas;
		ID2D1SolidColorBrush *fgBrush;
		IDWriteTextFormat *textFormat;
		IDWriteFactory *dWriteFactory;
	};
}
