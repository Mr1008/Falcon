#include "pch.h"
#include "TerminalCanvas.h"

namespace Controls
{
	TerminalCanvas::TerminalCanvas()
		: Direct2DCanvas(L"HwAcceleratedTerminal"),
		renderer(nullptr)
	{
	}

	void TerminalCanvas::registerRenderer(TerminalRenderer* renderer)
	{
		this->renderer = renderer;
	}

	void TerminalCanvas::render()
	{
		forceRender();
	}

	void TerminalCanvas::onRenderDxScene(ID2D1RenderTarget* target)
	{
		notifyRenderer([&](TerminalRenderer * r) {r->onRenderDxScene(target); });
	}

	void TerminalCanvas::onCreateDxResources(ID2D1RenderTarget* target)
	{
		notifyRenderer([&](TerminalRenderer * r) {r->onCreateDxResources(target); });
	}

	void TerminalCanvas::onReleaseDxResources()
	{
		Direct2DCanvas::onReleaseDxResources();
		notifyRenderer([&](TerminalRenderer * r) {r->onReleaseDxResources(); });
	}

	int TerminalCanvas::onResize(ResizeType type, const SIZE& size)
	{
		int ret = Direct2DCanvas::onResize(type, size);
		if (ret) return ret;

		notifyRenderer([&](TerminalRenderer * r) {r->onResizeScene(type, size); });

		return 0;
	}

	void TerminalCanvas::notifyRenderer(std::function<void(TerminalRenderer*)> fn)
	{
		if (renderer != nullptr) {
			fn(renderer);
		}
	}
}