#pragma once
#include "pch.h"
#include "InputListener.h"
#include <vector>
#include <functional>
#include "../Falcon.Shared/Publisher.h"

namespace Controls
{
	enum ResizeType {
		Unknown,
		Restored,
		Minimized,
		MaxShow,
		Maximized,
		MaxHide
	};

	class Control
		: private Shared::Publisher<InputListener>
	{
	public:
		Control(
			const std::wstring& className,
			const std::wstring& title,
			DWORD style,
			DWORD exStyle = 0,
			int x = CW_USEDEFAULT,
			int y = CW_USEDEFAULT,
			int width = CW_USEDEFAULT,
			int height = CW_USEDEFAULT);

		void addChild(Control& child, bool defaultFocus = false);
		void show(int mode = SW_SHOWNORMAL);
		void close();
		void focus();
		bool hasFocus();
		HWND getHwnd() const { return hwnd; }
		static void set_hInstance(HINSTANCE hInstance);
		void registerInputListener(InputListener* inputListener);
		void unregisterInputListener(InputListener* inputListener);

		virtual int onCreate();
		virtual void onCreated();
		virtual int onPaint();
		virtual int onDestroy();
		virtual int onResize(ResizeType type, const SIZE& size);
		virtual bool onMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

		friend LRESULT CALLBACK OnMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	protected:
		std::vector<Control*> children;
		bool supportsGDIPainting;

		void setControlPosAndSize(POINT* pos, const SIZE* size);
		void forceRender(const RECT* rect = nullptr);
		const SIZE getSize() const;
		const RECT getClientRect() const;
		const POINT getPos() const;

	private:
		HWND hwnd = nullptr;
		std::wstring className;
		static HINSTANCE hInstance;
		const Control* parent;
		Control* defaultChild;
		std::wstring title;
		DWORD style;
		DWORD exStyle;
		POINT pos;
		SIZE size;

		void setParent(Control* parent);
		void createAndRegisterClass();
		void createControl();
	};
}
