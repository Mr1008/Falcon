#include "pch.h"
#include <string>
#include <thread>
#include "DWriteFontLoader.h"
#include "DxTerminalRenderer.h"


namespace Engine
{
    using namespace std;
    using namespace Fonts;
    using namespace Controls;
    using namespace Microsoft::WRL;

    DxTerminalRenderer::DxTerminalRenderer(TerminalBuffer* textBuffer) :
        textBuffer(textBuffer)
    {
    }

    void DxTerminalRenderer::onCreateDxResources(ID2D1DeviceContext* dc)
    {
        HRESULT hr = DWriteCreateFactory(
            DWRITE_FACTORY_TYPE_SHARED,
            __uuidof(IDWriteFactory),
            reinterpret_cast<IUnknown * *>(dWriteFactory.GetAddressOf())
        );

        loadFont(hr);
        const D2D1_COLOR_F color = D2D1::ColorF(1.0f, 1.0f, 1.0f, 0.0f);
        dc->CreateSolidColorBrush(color, &fgBrush);
        dc->CreateSolidColorBrush(color, &bgBrush);
        isReadyFlag = true;
    }

    void DxTerminalRenderer::onReleaseDxResources()
    {
        isReadyFlag = false;
    }

    void DxTerminalRenderer::onResizeScene(ResizeType type, const SIZE& size)
    {
        sceneSize = size;
        const COORD oldSize = sizeInCharacters;
        sizeInCharacters = countSizeInCharacters();
        if (oldSize.X != sizeInCharacters.X || oldSize.Y != sizeInCharacters.Y) {
            notifyListeners([&](RendererEventsListener* listener) {listener->onTerminalSizeChange(sizeInCharacters); });
            textBuffer->inOwnedContext([this]() {
                textBuffer->clear();
                }
            );

        }
    }

    bool DxTerminalRenderer::isReady() const
    {
        return isReadyFlag;
    }

    void DxTerminalRenderer::init(function<void()> render)
    {
        thread cursorThread = thread([this](function<void()> render) {
            while (true) {
                cursorBlink = !cursorBlink;
                render();
                Sleep(500);
            }
            }, move(render));

        cursorThread.detach();
    }

    void DxTerminalRenderer::onRender(ID2D1DeviceContext* dc)
    {
        dc->Clear(D2D1::ColorF(0.1882f, 0.0392f, 0.1412f, 0.95f));
        dc->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_CLEARTYPE);
        textBuffer->inOwnedContext([&]() {
            size_t linesCount = textBuffer->getLinesCount();
            size_t linesToDisplay = min(sizeInCharacters.Y, linesCount);
            for (size_t y = 0; y < linesToDisplay; ++y) {
                auto& line = textBuffer->getLine(linesCount - linesToDisplay + y);
                for (size_t x = 0; x < line.size(); ++x) {
                    const TerminalCharacter& character = line[x];
                    dc->SetTransform(D2D1::Matrix3x2F::Translation(x * textMetrics.width, y * textMetrics.height));
                    if (character.backgroundColor.a != 0) {
                        bgBrush->SetColor(D2D1::ColorF((character.backgroundColor.r << 16) + (character.backgroundColor.g << 8) + (character.backgroundColor.b), character.backgroundColor.a / 255.f));
                        dc->FillRectangle(D2D1::RectF(0, 0, textMetrics.width, textMetrics.height), bgBrush.Get());
                    }
                    if (character.character != L' ') {
                        fgBrush->SetColor(D2D1::ColorF((character.foregroundColor.r << 16) + (character.foregroundColor.g << 8) + (character.foregroundColor.b), character.foregroundColor.a / 255.f));
                        dc->DrawText(
                            &character.character,
                            1,
                            textFormat.Get(),
                            D2D1::RectF(),
                            fgBrush.Get());
                    }
                }
            }
            }
        );
        if ((cursorBlink || !textBuffer->isCursorBlinking()) && textBuffer->isCursorVisible()) {
            const POINT& position = textBuffer->getCursorPosition();
            dc->SetTransform(D2D1::Matrix3x2F::Translation(position.x * textMetrics.width, position.y * textMetrics.height));
            dc->FillRectangle(D2D1::RectF(0, 0, 3, textMetrics.height), fgBrush.Get());
        }
    }

    void DxTerminalRenderer::loadFont(HRESULT& hr)
    {
        DWriteFontLoader fontLoader(dWriteFactory.Get());
        std::vector<std::wstring> filePaths;
        wchar_t path[_MAX_PATH];
        if (_wfullpath(path, L"fonts/UbuntuMono-R.ttf", _MAX_PATH))
        {
            filePaths.push_back(wstring(path));
            ComPtr<IDWriteFontCollection> collection;
            hr = fontLoader.CreateFontCollection(filePaths, collection.GetAddressOf());
            hr = dWriteFactory->CreateTextFormat(
                L"Ubuntu Mono",
                collection.Get(),
                DWRITE_FONT_WEIGHT_NORMAL,
                DWRITE_FONT_STYLE_NORMAL,
                DWRITE_FONT_STRETCH_NORMAL,
                20,
                L"",
                &textFormat
            );
            calculateCharWidth();
        }
    }

    void DxTerminalRenderer::calculateCharWidth()
    {
        ComPtr<IDWriteTextLayout> textLayout;
        HRESULT hr = dWriteFactory->CreateTextLayout(L"X", 1, textFormat.Get(), 100, 100, textLayout.GetAddressOf());
        if (SUCCEEDED(hr)) {
            textLayout->GetMetrics(&textMetrics);
        }
    }

    COORD DxTerminalRenderer::countSizeInCharacters()
    {
        return {
            static_cast<short>(sceneSize.cx / textMetrics.width),
            static_cast<short>(sceneSize.cy / textMetrics.height)
        };
    }
}