#pragma once
#include "pch.h"
#include "InputListener.h"
#include <vector>
#include <functional>

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
		void registerInputListener(InputListener* listener);
		HWND getHwnd() const { return hwnd; }
		static void set_hInstance(HINSTANCE hInstance);

		virtual int onCreate();
		virtual void onCreated();
		virtual int onPaint(HDC hdc, PAINTSTRUCT* ps);
		virtual int onDestroy();
		virtual int onResize(ResizeType type, const SIZE& size);

		friend LRESULT CALLBACK OnMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	protected:
		bool hasCustomPaint = false;
		std::vector<Control*> children;

		void setControlPosAndSize(POINT* pos, const SIZE* size);
		void forceRender(const RECT* rect = nullptr);
		const SIZE& getSize() const;
		const POINT& getPos() const;

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
		std::vector<InputListener*> inputListeners;


		void setParent(Control* parent);
		void createAndRegisterClass();
		void createControl();
		void forEachInputListener(std::function<void(InputListener*)> fn);
	};
}
