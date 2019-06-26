#pragma once
#include <d2d1_2.h>
#include <wrl.h>
#include <chrono>
#include "../Falcon.UI/TerminalRenderer.h"
#include "TerminalBuffer.h"
#include "RendererEventsListener.h"

namespace Engine
{
    class DxTerminalRenderer :
        public Controls::TerminalRenderer,
        public Shared::Publisher<RendererEventsListener>

    {
    public:
        DxTerminalRenderer(TerminalBuffer* textBuffer);
        bool isReady() const;
        void init(std::function<void()> render);

        virtual void onCreateDxResources(ID2D1DeviceContext* dc);
        virtual void onReleaseDxResources();
        virtual void onResizeScene(Controls::ResizeType type, const SIZE& size);
        virtual void onRender(ID2D1DeviceContext* dc);

    private:
        Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> fgBrush;
        Microsoft::WRL::ComPtr<IDWriteTextFormat> textFormat;
        Microsoft::WRL::ComPtr<IDWriteFontFile> fontFile;
        Microsoft::WRL::ComPtr<IDWriteFontFace> fontFace;
        Microsoft::WRL::ComPtr<IDWriteFactory> dWriteFactory;
        TerminalBuffer* textBuffer;
        bool isReadyFlag;
        DWRITE_TEXT_METRICS textMetrics;
        const D2D1_RECT_F padding = D2D1::RectF(10.f, 10.f, 10.f, 10.f);
        SIZE sceneSize;
        COORD sizeInCharacters;
        bool cursorBlink = false;

        void loadFont(HRESULT& hr);
        void calculateCharWidth();
        COORD countSizeInCharacters();
    };
}
