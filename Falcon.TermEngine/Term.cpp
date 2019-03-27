#include "pch.h"
#include "Term.h"
#pragma comment(lib, "dwrite")

using namespace Controls;
namespace Engine
{
	std::wstring t;

	Term::Term(Controls::TerminalCanvas *canvas)
		: canvas(canvas)
	{
		canvas->registerInputListener(this);
		canvas->registerRenderer(this);
	}

	void Term::onMouseMoved(const POINT &pos)
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

	void Term::onMouseButtonDown(const POINT &pos, MouseButton button)
	{
		if (!canvas->hasFocus()) {
			canvas->focus();
		}
	}

	void Term::onMouseButtonUp(const POINT &pos, MouseButton button)
	{
	}

	void Term::onRenderDxScene(ID2D1RenderTarget *target)
	{
		target->Clear(D2D1::ColorF(D2D1::ColorF::Black));
		
		auto r = target->GetSize();
		target->DrawText(t.c_str(), t.size(), textFormat, D2D1::RectF(10, 10, r.width - 20, r.height - 20), fgBrush);
	}

	void Term::onCreateDxResources(ID2D1RenderTarget *target)
	{
		HRESULT hr = DWriteCreateFactory(
			DWRITE_FACTORY_TYPE_SHARED,
			__uuidof(IDWriteFactory),
			reinterpret_cast<IUnknown**>(&dWriteFactory)
		);
		hr = dWriteFactory->CreateTextFormat(
			L"Consolas",
			nullptr,
			DWRITE_FONT_WEIGHT_NORMAL,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			16,
			L"",
			&textFormat
		);
		const D2D1_COLOR_F color = D2D1::ColorF(1.0f, 1.0f, 1.0f);
		target->CreateSolidColorBrush(color, &fgBrush);
	}

	void Term::onReleaseDxResources()
	{
		releaseDxResource(&fgBrush);
		releaseDxResource(&textFormat);
		releaseDxResource(&dWriteFactory);
	}

	int Term::onResizeScene(ResizeType type, const SIZE &size)
	{
		return 0;
	}

	template <class T>
	void Term::releaseDxResource(T **resource)
	{
		(*resource)->Release();
		*resource = nullptr;
	}
}
