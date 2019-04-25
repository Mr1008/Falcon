#include "pch.h"
#include "TerminalWindowController.h"
#include "../Falcon.UI/MessagePipe.h"
#include "DxCommon.h"
#include "DWriteFontLoader.h"


namespace Engine
{
	using namespace std;
	using namespace Controls;
	using namespace Fonts;
	using namespace Messages;
	using namespace Shared;
	using namespace Microsoft::WRL;

	TerminalWindowController::TerminalWindowController() :
		window(L"Falcon"),
		charWidth(-1.f),
		dWriteFactory(nullptr),
		fgBrush(nullptr),
		fontFace(nullptr),
		fontFile(nullptr),
		sceneSize({ 0,0 }),
		textFormat(nullptr),
		isWindowUp(false)
	{
		window.registerInputListener(this);
		window.registerTerminalRenderer(this);
	}

	void TerminalWindowController::show()
	{
		isWindowUp = true;
		window.show();
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

	void TerminalWindowController::registerTerminalWindowListener(TerminalWindowListener* listener)
	{
		Publisher<TerminalWindowListener>::registerListener(listener);
	}

	void TerminalWindowController::unregisterTerminalWindowListener(TerminalWindowListener* listener)
	{
		Publisher<TerminalWindowListener>::unregisterListener(listener);
	}

	void TerminalWindowController::onMouseMoved(const POINT& pos)
	{
	}

	void TerminalWindowController::renderTerminal()
	{
		window.render(
			[this](ID2D1DeviceContext * dc)
			{
				dc->Clear(D2D1::ColorF(0.1882f, 0.0392f, 0.1412f, 0.95f));

				auto r = dc->GetSize();
				dc->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_CLEARTYPE);
				dc->DrawText(
					L"TEST1234567890",
					14,
					textFormat.Get(),
					D2D1::RectF(padding.left, padding.top, r.width - padding.left - padding.right, r.height - padding.top - padding.bottom),
					fgBrush.Get()
				);
			}
		);
	}

	void TerminalWindowController::onKeyPushed(wchar_t key, bool isFirstOccurence, unsigned int repeatCount)
	{
	}

	void TerminalWindowController::onMouseButtonDown(const POINT& pos, MouseButton button)
	{
	}

	void TerminalWindowController::onMouseButtonUp(const POINT& pos, MouseButton button)
	{
	}

	void TerminalWindowController::onCreateDxResources(ID2D1DeviceContext * dc)
	{
		HRESULT hr = DWriteCreateFactory(
			DWRITE_FACTORY_TYPE_SHARED,
			__uuidof(IDWriteFactory),
			reinterpret_cast<IUnknown **>(dWriteFactory.GetAddressOf())
		);

		loadFont(hr);
		const D2D1_COLOR_F color = D2D1::ColorF(1.0f, 1.0f, 1.0f);
		dc->CreateSolidColorBrush(color, &fgBrush);
	}

	void TerminalWindowController::loadFont(HRESULT & hr)
	{
		DWriteFontLoader fontLoader(dWriteFactory.Get());
		std::vector<std::wstring> filePaths;
		wchar_t path[_MAX_PATH];
		if (_wfullpath(path, L"fonts/UbuntuMono-R.ttf", _MAX_PATH))
		{
			filePaths.push_back(wstring(path));
			IDWriteFontCollection* collection;
			hr = fontLoader.CreateFontCollection(filePaths, &collection);
			hr = dWriteFactory->CreateTextFormat(
				L"Ubuntu Mono",
				collection,
				DWRITE_FONT_WEIGHT_NORMAL,
				DWRITE_FONT_STYLE_NORMAL,
				DWRITE_FONT_STRETCH_NORMAL,
				21,
				L"",
				&textFormat
			);
			SafeRelease(&collection);
			calculateCharWidth();
		}
	}

	void TerminalWindowController::onReleaseDxResources()
	{
	}

	int TerminalWindowController::onResizeScene(ResizeType type, const SIZE & size)
	{
		sceneSize = size;
		notifyListeners([&](TerminalWindowListener * listener) {listener->onWindowResize(countSizeInCharacters(size)); });
		renderTerminal();
		return 0;
	}

	void TerminalWindowController::calculateCharWidth()
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

	COORD TerminalWindowController::countSizeInCharacters(SIZE sizeInPx)
	{
		return {
			(short)sizeInPx.cx / 20,
			(short)sizeInPx.cy / 20
		};
	}
}
