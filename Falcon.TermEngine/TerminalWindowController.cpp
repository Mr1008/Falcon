#include "pch.h"
#include "TerminalWindowController.h"
#include "../Falcon.UI/MessagePipe.h"
#include "DxCommon.h"
#include "DWriteFontLoader.h"
#pragma comment(lib, "dwrite")


namespace Engine
{
	using namespace std;
	using namespace Controls;
	using namespace Fonts;
	using namespace Messages;

	TerminalWindowController::TerminalWindowController() :
		window(L"Falcon"),
		charWidth(-1.f),
		dWriteFactory(nullptr),
		fgBrush(nullptr),
		fontFace(nullptr),
		fontFile(nullptr),
		sceneSize({ 0,0 }),
		textFormat(nullptr),
		isWindowUp(false),
		listener(nullptr)
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
		this->listener = listener;
	}

	void TerminalWindowController::onMouseMoved(const POINT& pos)
	{
		window.render(
			[this](ID2D1DeviceContext * dc) 
			{
				dc->Clear(D2D1::ColorF(0.1882f, 0.0392f, 0.1412f, 0.7f));

				auto r = dc->GetSize();
				dc->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_CLEARTYPE);
				dc->DrawText(
					L"TEST",
					4,
					textFormat,
					D2D1::RectF(padding.left, padding.top, r.width - padding.left - padding.right, r.height - padding.top - padding.bottom),
					fgBrush
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
			reinterpret_cast<IUnknown * *>(&dWriteFactory)
		);

		loadFont(hr);
		const D2D1_COLOR_F color = D2D1::ColorF(1.0f, 1.0f, 1.0f);
		dc->CreateSolidColorBrush(color, &fgBrush);
	}

	void TerminalWindowController::loadFont(HRESULT & hr)
	{
		DWriteFontLoader fontLoader(dWriteFactory);
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
		SafeRelease(&fgBrush);
		SafeRelease(&fontFile);
		SafeRelease(&fontFace);
		SafeRelease(&textFormat);
		SafeRelease(&dWriteFactory);
	}

	int TerminalWindowController::onResizeScene(ResizeType type, const SIZE & size)
	{
		sceneSize = size;
		notifyListener([&](TerminalWindowListener * listener) {listener->onWindowResize(countSizeInCharacters(size)); });
		return 0;
	}

	void TerminalWindowController::calculateCharWidth()
	{
		IDWriteTextLayout* textLayout;
		HRESULT hr = dWriteFactory->CreateTextLayout(L"X", 1, textFormat, 100, 100, &textLayout);
		if (SUCCEEDED(hr)) {
			hr = textLayout->DetermineMinWidth(&charWidth);
			if (FAILED(hr)) {
				charWidth = -1.f;
			}

			SafeRelease(&textLayout);
		}
	}
	void TerminalWindowController::notifyListener(std::function<void(TerminalWindowListener*)> fn)
	{
		if (listener != nullptr) {
			fn(listener);
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
