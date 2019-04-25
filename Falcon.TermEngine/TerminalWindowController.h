#pragma once
#include <dwrite.h>
#include <functional>
#include <wrl.h>
#include "../Falcon.UI/InputListener.h"
#include "../Falcon.UI/TerminalRenderer.h"
#include "../Falcon.UI/MainWindow.h"
#include "TerminalWindowListener.h"

namespace Engine
{
	class TerminalWindowController :
		public Controls::TerminalRenderer,
		public Controls::InputListener,
		private Shared::Publisher<TerminalWindowListener>
	{
	public:
		TerminalWindowController();
		void show();
		void close();
		bool isUp() const;
		void registerTerminalWindowListener(TerminalWindowListener* listener);
		void unregisterTerminalWindowListener(TerminalWindowListener* listener);

		virtual void onMouseMoved(const POINT& pos);
		void renderTerminal();
		virtual void onKeyPushed(wchar_t key, bool isFirstOccurence, unsigned int repeatCount);
		virtual void onMouseButtonDown(const POINT& pos, Controls::MouseButton button);
		virtual void onMouseButtonUp(const POINT& pos, Controls::MouseButton button);
		virtual void onCreateDxResources(ID2D1DeviceContext* dc);
		virtual void onReleaseDxResources();
		virtual int onResizeScene(Controls::ResizeType type, const SIZE& size);

	private:
		Controls::MainWindow window;
		Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> fgBrush;
		Microsoft::WRL::ComPtr<IDWriteTextFormat> textFormat;
		Microsoft::WRL::ComPtr<IDWriteFontFile> fontFile;
		Microsoft::WRL::ComPtr<IDWriteFontFace> fontFace;
		Microsoft::WRL::ComPtr<IDWriteFactory> dWriteFactory;
		float charWidth;
		const D2D1_RECT_F padding = D2D1::RectF(10.f, 10.f, 10.f, 10.f);
		SIZE sceneSize;
		bool isWindowUp;

		void loadFont(HRESULT& hr);
		void calculateCharWidth();
		COORD countSizeInCharacters(SIZE sizeInPx); // Temporary, should go to renderer. Mock, counts incorrectly.
	};
}
