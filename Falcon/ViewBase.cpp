#include "pch.h"
#include "ViewBase.h"

using namespace Views;
using namespace std;

HINSTANCE ViewBase::hInstance;

LRESULT CALLBACK Views::OnMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_CREATE)
	{
		CREATESTRUCT *pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
		ViewBase* view = reinterpret_cast<ViewBase*>(pCreate->lpCreateParams);
		SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(view));
		return 0;
	}

	ViewBase *view = reinterpret_cast<ViewBase*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

	switch (uMsg)
	{
	case WM_DESTROY:
		view->onDestroy();
		return 0;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);
		view->onPaint(hdc, &ps);
		EndPaint(hwnd, &ps);
		return 0;
	}
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

ViewBase::ViewBase(
	const wstring &className, 
	const ViewBase * parent, 
	const wstring &title, 
	DWORD style,
	int x, int y, int width, int height
)
{
	CreateAndRegisterClass(className);
	MakeWindow(parent, title.c_str(), style, x, y, width, height);
}

void ViewBase::show(int mode) const
{
	ShowWindow(hwnd, mode);
}

void ViewBase::set_hInstance(HINSTANCE hInstance)
{
	ViewBase::hInstance = hInstance;
}

void Views::ViewBase::onDestroy()
{
	PostQuitMessage(0);
}

void ViewBase::CreateAndRegisterClass(const wstring &className)
{
	this->className = className.c_str();
	WNDCLASS wc = { };

	wc.lpfnWndProc = Views::OnMessage;
	wc.hInstance = ViewBase::hInstance;
	wc.lpszClassName = this->className;

	RegisterClass(&wc);
}

void ViewBase::MakeWindow(
	const ViewBase* parent,
	LPCWSTR title,
	DWORD style,
	int x,
	int y,
	int width,
	int height
)
{
	hwnd = CreateWindowEx(
		0,
		this->className,
		title,
		style,
		x, y, width, height,
		parent ? parent->getHwnd() : nullptr, // parent
		nullptr,
		ViewBase::hInstance,
		this
	);

	if (hwnd == NULL) {
		throw std::exception("Could not create window");
	}
}
