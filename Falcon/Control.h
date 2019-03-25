#pragma once
#include "pch.h"
#include <vector>

namespace Controls
{
	enum ResizeType {
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
			const std::wstring &className,
			const std::wstring &title,
			DWORD style,
			int x = CW_USEDEFAULT,
			int y = CW_USEDEFAULT,
			int width = CW_USEDEFAULT,
			int height = CW_USEDEFAULT) :
			className(className), title(title), style(style), pos({x, y}), size({width, height})
		{
		}
		
		void addChild(Control &child);
		void show(int mode = SW_SHOWNORMAL);
		HWND getHwnd() const { return hwnd; }
		static void set_hInstance(HINSTANCE hInstance);

		virtual int onCreate();
		virtual int onPaint(HDC hdc, PAINTSTRUCT *ps);
		virtual int onDestroy();
		virtual int onResize(ResizeType type, const SIZE &size);

		friend LRESULT CALLBACK OnMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	protected:
		bool hasCustomPaint = false;
		std::vector<Control*> children;
		
		void setControlPosAndSize(POINT *pos, const SIZE *size);
		void forceRender(const RECT *rect = nullptr);
		const SIZE& getSize() const;
		const POINT& getPos() const;

	private:
		HWND hwnd = nullptr;
		std::wstring className;
		static HINSTANCE hInstance;
		const Control *parent;
		std::wstring title;
		DWORD style;
		POINT pos;
		SIZE size;

		void setParent(Control *parent);
		void createAndRegisterClass();
		void createControl();
	};
}
