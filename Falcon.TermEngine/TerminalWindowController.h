#pragma once
#include <dwrite.h>
#include <functional>
#include "../Falcon.UI/ControlEventsListener.h"
#include "../Falcon.UI/MainWindow.h"
#include "DxTerminalRenderer.h"
#include "TerminalWindowActions.h"

namespace Engine
{
    class TerminalWindowController :
        public Controls::ControlEventListener,
        public TerminalWindowActions
    {
    public:
        TerminalWindowController(std::unique_ptr<DxTerminalRenderer>&& renderer, std::function<void(wchar_t)> sendInput);

        void show();
        void close();
        bool isUp() const;
        bool isUpAndRendererReady() const;
        void render();
        
        virtual void setTitle(const std::wstring& title);
        virtual void onMouseMoved(const POINT& pos);
        virtual void onKeyPushed(wchar_t key, bool isFirstOccurence, unsigned int repeatCount);
        virtual void onMouseButtonDown(const POINT& pos, Controls::MouseButton button);
        virtual void onMouseButtonUp(const POINT& pos, Controls::MouseButton button);
        virtual void onSizeChanged(Controls::ResizeType type, const SIZE& size);

    private:
        Controls::MainWindow window;
        bool isWindowUp;
        std::unique_ptr<DxTerminalRenderer> renderer;
        std::function<void(wchar_t)> sendInput;
    };
}
