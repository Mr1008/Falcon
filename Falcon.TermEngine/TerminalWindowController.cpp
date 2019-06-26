#include "pch.h"
#include "TerminalWindowController.h"
#include "../Falcon.UI/MessagePipe.h"

namespace Engine
{
    using namespace std;
    using namespace Controls;
    using namespace Messages;

    TerminalWindowController::TerminalWindowController(unique_ptr<DxTerminalRenderer>&& renderer, std::function<void(wchar_t)> writeOutput) :
        window(L"Falcon"),
        isWindowUp(false),
        renderer(move(renderer)),
        writeOutput(writeOutput)
    {
        window.registerEventListener(this);
        window.registerTerminalRenderer(this->renderer.get());
    }

    void TerminalWindowController::show()
    {
        window.show();
        isWindowUp = true;
        renderer->init([&]() {render(); });
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
        writeOutput(key);
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

    void TerminalWindowController::onSizeChanged(ResizeType type, const SIZE& size)
    {
    }

    void TerminalWindowController::render()
    {
        window.invalidate();
    }
}
