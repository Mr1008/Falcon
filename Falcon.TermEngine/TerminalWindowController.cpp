#include "pch.h"
#include "TerminalWindowController.h"
#include "../Falcon.UI/MessagePipe.h"

namespace Engine
{
	using namespace std;
	using namespace Controls;
	using namespace Messages;

	TerminalWindowController::TerminalWindowController(unique_ptr<DxTerminalRenderer>&& renderer) :
		window(L"Falcon"),
		isWindowUp(false),
		renderer(move(renderer))
	{
		window.registerEventListener(this);
		window.registerTerminalRenderer(this->renderer.get());
	}

	void TerminalWindowController::show()
	{
		window.show();
		isWindowUp = true;
		MessagePipe::start();
		isWindowUp = false;
	}

	void TerminalWindowController::close()
	{
		window.close();
	}

	bool TerminalWindowController::isUp() const
	{
		return isWindowUp;
	}

	bool TerminalWindowController::isUpAndRendererReady() const
	{
		return isUp() && renderer->isReady();
	}

	void TerminalWindowController::onKeyPushed(wchar_t key, bool isFirstOccurence, unsigned int repeatCount)
	{
	}

	void TerminalWindowController::onMouseMoved(const POINT& pos)
	{
	}

	void TerminalWindowController::onMouseButtonDown(const POINT& pos, MouseButton button)
	{
	}

	void TerminalWindowController::onMouseButtonUp(const POINT& pos, MouseButton button)
	{
	}

	void TerminalWindowController::onSizeChanged(Controls::ResizeType type, const SIZE& size)
	{
		render(); // TODO: Unify logic as this should go to renderer IMO
	}

	void TerminalWindowController::render()
	{
		window.invalidate([this](ID2D1DeviceContext * dc) {renderer->render(dc); });
	}
}
