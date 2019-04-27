#include "pch.h"
#include <string>
#include "DWriteFontLoader.h"
#include "DxTerminalRenderer.h"


namespace Engine
{
	using namespace std;
	using namespace Fonts;
	using namespace Controls;
	using namespace Microsoft::WRL;

	DxTerminalRenderer::DxTerminalRenderer(TextBuffer* textBuffer) :
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
		const D2D1_COLOR_F color = D2D1::ColorF(1.0f, 1.0f, 1.0f);
		dc->CreateSolidColorBrush(color, &fgBrush);
		isReadyFlag = true;
	}

	void DxTerminalRenderer::onReleaseDxResources()
	{
		isReadyFlag = false;
	}

	void DxTerminalRenderer::onResizeScene(ResizeType type, const SIZE& size)
	{
		sceneSize = size;
		notifyListeners([&](RendererEventsListener * listener) {listener->onTerminalSizeChange(countSizeInCharacters(size)); });
	}

	bool DxTerminalRenderer::isReady() const
	{
		return isReadyFlag;
	}

	void DxTerminalRenderer::render(ID2D1DeviceContext* dc)
	{
		dc->Clear(D2D1::ColorF(0.1882f, 0.0392f, 0.1412f, 0.95f));

		auto r = dc->GetSize();
		dc->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_CLEARTYPE);
		size_t linesCount = textBuffer->getLinesCount();
		//for (size_t i = 0; i < linesCount; ++i) {
		dc->DrawText(
			textBuffer->getLine(0).c_str(),
			textBuffer->getLine(0).size(),
			textFormat.Get(),
			D2D1::RectF(padding.left, padding.top, r.width - padding.left - padding.right, r.height - padding.top - padding.bottom),
			fgBrush.Get()
		);
		//}
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
				21,
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
			hr = textLayout->DetermineMinWidth(&charWidth);
			if (FAILED(hr)) {
				charWidth = -1.f;
			}
		}
	}

	COORD DxTerminalRenderer::countSizeInCharacters(SIZE sizeInPx)
	{
		return {
			(short)sizeInPx.cx / 20,
			(short)sizeInPx.cy / 20
		};
	}
}