#pragma once
#include "Direct2DCanvas.h"
#include "TerminalRenderer.h"
#include <functional>

namespace Controls
{
	class TerminalCanvas :
		public Direct2DCanvas
	{
	public:
		TerminalCanvas();

		void registerRenderer(TerminalRenderer *renderer);
		void render();

		virtual void onRenderDxScene(ID2D1RenderTarget *target);
		virtual void onCreateDxResources(ID2D1RenderTarget *target);
		virtual void onReleaseDxResources();
		virtual int onResize(ResizeType type, const SIZE &size);
	private:
		TerminalRenderer *renderer;
		void notifyRenderer(std::function<void(TerminalRenderer*)> fn);
	};
}
