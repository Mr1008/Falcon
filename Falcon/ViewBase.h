#pragma once
#include "pch.h"
#include <vector>

namespace Views
{
	class ViewBase
	{
	public:
		ViewBase(
			const std::wstring &className,
			const std::wstring &title,
			DWORD style,
			int x = CW_USEDEFAULT,
			int y = CW_USEDEFAULT,
			int width = CW_USEDEFAULT,
			int height = CW_USEDEFAULT) :
			className(className), title(title), style(style), x(x), y(y), width(width), height(height)
		{
		}
		
		void addChild(ViewBase &child);
		void setParent(ViewBase *parent);
		void show(int mode = SW_SHOWNORMAL);
		HWND getHwnd() const { return hwnd; }
		static void set_hInstance(HINSTANCE hInstance);

		friend LRESULT CALLBACK OnMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	protected:
		bool hasCustomPaint = false;

		virtual void onCreate();
		virtual void onPaint(HDC hdc, PAINTSTRUCT *ps);
		virtual void onDestroy();

	private:
		std::vector<ViewBase*> children;
		HWND hwnd = nullptr;
		std::wstring className;
		static HINSTANCE hInstance;
		const ViewBase *parent;
		std::wstring title;
		DWORD style;
		int x;
		int y;
		int width;
		int height;

		void CreateAndRegisterClass();
		void MakeWindow();
	};
}
