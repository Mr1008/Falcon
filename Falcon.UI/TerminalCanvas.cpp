#include "pch.h"
#include "TerminalCanvas.h"

namespace Controls
{
	TerminalCanvas::TerminalCanvas()
		: Direct2DCanvas(L"HwAcceleratedTerminal")
	{
	}

	void TerminalCanvas::registerRenderer(TerminalRenderer * renderer)
	{
		this->renderer = renderer;
	}

	void TerminalCanvas::render()
	{
		forceRender();
	}

	void TerminalCanvas::onRenderDxScene(ID2D1RenderTarget *target)
	{
		if (renderer) {
			renderer->onRenderDxScene(target);
		}
	}

	void TerminalCanvas::onCreateDxResources(ID2D1RenderTarget *target)
	{
		if (renderer) {
			renderer->onCreateDxResources(target);
		}
	}

	void TerminalCanvas::onReleaseDxResources()
	{
		Direct2DCanvas::onReleaseDxResources();

		if (renderer) {
			renderer->onReleaseDxResources();
		}
	}

	int TerminalCanvas::onResize(ResizeType type, const SIZE &size)
	{
		int ret = Direct2DCanvas::onResize(type, size);
		if (ret) return ret;

		if (renderer) {
			return renderer->onResizeScene(type, size);
		}

		return 0;
	}
}