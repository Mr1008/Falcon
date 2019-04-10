#include "pch.h"
#include "Term.h"
#include "DxCommon.h"
#include "DWriteFontLoader.h"
#pragma comment(lib, "dwrite")

namespace Engine
{
	using namespace Controls;
	using namespace Fonts;
	using namespace std;
	wstring t;

	Term::Term(Controls::TerminalCanvas* canvas)
		: canvas(canvas),
		charWidth(-1.f),
		dWriteFactory(nullptr),
		fgBrush(nullptr),
		fontFace(nullptr),
		fontFile(nullptr),
		sceneSize({ 0,0 }),
		textFormat(nullptr)
	{
		canvas->registerInputListener(this);
		canvas->registerRenderer(this);
	}

	void Term::onMouseMoved(const POINT& pos)
	{
	}

	void Term::onKeyPushed(wchar_t key, bool isFirstOccurence, unsigned int repeatCount)
	{
		if (key == '\b' && t.size()) {
			t.pop_back();
		}
		else {
			t += key;
		}
		canvas->render();
	}

	void Term::onMouseButtonDown(const POINT& pos, MouseButton button)
	{
		if (!canvas->hasFocus()) {
			canvas->focus();
		}
	}

	void Term::onMouseButtonUp(const POINT& pos, MouseButton button)
	{
	}

	void Term::onRenderDxScene(ID2D1RenderTarget* target)
	{
		target->Clear(D2D1::ColorF(0.1882f, 0.0392f, 0.1412f));

		auto r = target->GetSize();
		target->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_CLEARTYPE);
		target->DrawText(
			t.c_str(),
			t.size(),
			textFormat,
			D2D1::RectF(padding.left, padding.top, r.width - padding.left - padding.right, r.height - padding.top - padding.bottom),
			fgBrush
		);
	}

	void Term::onCreateDxResources(ID2D1RenderTarget * target)
	{
		HRESULT hr = DWriteCreateFactory(
			DWRITE_FACTORY_TYPE_SHARED,
			__uuidof(IDWriteFactory),
			reinterpret_cast<IUnknown **>(&dWriteFactory)
		);

		loadFont(hr);
		const D2D1_COLOR_F color = D2D1::ColorF(1.0f, 1.0f, 1.0f);
		target->CreateSolidColorBrush(color, &fgBrush);
	}

	void Term::loadFont(HRESULT& hr)
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

	void Term::onReleaseDxResources()
	{
		SafeRelease(&fgBrush);
		SafeRelease(&fontFile);
		SafeRelease(&fontFace);
		SafeRelease(&textFormat);
		SafeRelease(&dWriteFactory);
	}

	int Term::onResizeScene(ResizeType type, const SIZE & size)
	{
		sceneSize = size;
		return 0;
	}

	void Term::calculateCharWidth()
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
}
