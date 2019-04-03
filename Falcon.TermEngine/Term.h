#pragma once
#include <vector>
#include <dwrite.h>
#include "../Falcon.UI/TerminalCanvas.h"
#include "../Falcon.UI/InputListener.h"
#include "../Falcon.UI/Mouse.h"
#include "TextBuffer.h"

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
		IDWriteFontFile *fontFile;
		IDWriteFontFace *fontFace;
		IDWriteFactory *dWriteFactory;
		std::unique_ptr<TextBuffer> textBuffer;
		float charWidth;
		const D2D1_RECT_F padding = D2D1::RectF(10.f, 10.f, 10.f, 10.f);
		SIZE sceneSize;

		void calculateCharWidth();
	};
}

