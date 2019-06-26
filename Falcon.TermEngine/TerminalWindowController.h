#pragma once
#include <dwrite.h>
#include <functional>
#include "../Falcon.UI/ControlEventsListener.h"
#include "../Falcon.UI/MainWindow.h"
#include "DxTerminalRenderer.h"

namespace Engine
{
    class TerminalWindowController :
        public Controls::ControlEventListener
    {
    public:
        TerminalWindowController(std::unique_ptr<DxTerminalRenderer>&& renderer, std::function<void(wchar_t)> writeOutput);

        void show();
        void close();
        bool isUp() const;
        bool isUpAndRendererReady() const;
        void render();

        virtual void onMouseMoved(const POINT& pos);
        virtual void onKeyPushed(wchar_t key, bool isFirstOccurence, unsigned int repeatCount);
        virtual void onMouseButtonDown(const POINT& pos, Controls::MouseButton button);
        virtual void onMouseButtonUp(const POINT& pos, Controls::MouseButton button);
        virtual void onSizeChanged(Controls::ResizeType type, const SIZE& size);

    private:
        Controls::MainWindow window;
        bool isWindowUp;
        std::unique_ptr<DxTerminalRenderer> renderer;
        std::function<void(wchar_t)> writeOutput;
    };
}
