#pragma once
#include "pch.h"
#include <string>

namespace Views
{
	class ViewBase
	{
	public:
		ViewBase(
			const std::wstring &className,
			const ViewBase* parent,
			const std::wstring &title,
			DWORD style,
			int x = CW_USEDEFAULT,
			int y = CW_USEDEFAULT,
			int width = CW_USEDEFAULT,
			int height = CW_USEDEFAULT);

		void show(int mode = SW_SHOWNORMAL) const;
		HWND getHwnd() const { return hwnd; }
		static void set_hInstance(HINSTANCE hInstance);

		friend LRESULT CALLBACK OnMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	protected:
		virtual void onPaint(HDC hdc, PAINTSTRUCT *ps) = 0;
		virtual void onDestroy();

	private:
		HWND hwnd;
		LPCWSTR className;
		static HINSTANCE hInstance;

		void CreateAndRegisterClass(const std::wstring &className);
		void MakeWindow(
			const ViewBase* parent,
			LPCWSTR title,
			DWORD style,
			int x,
			int y,
			int width,
			int height);
	};
}
