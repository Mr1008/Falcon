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
		view->onCreate();
		return 0;
	}

	ViewBase *view = reinterpret_cast<ViewBase*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

	switch (uMsg)
	{
	case WM_DESTROY:
		view->onDestroy();
		return 0;

	case WM_PAINT:
		if (view->hasCustomPaint)
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hwnd, &ps);
			view->onPaint(hdc, &ps);
			EndPaint(hwnd, &ps);
			return 0;
		}
		break;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void Views::ViewBase::addChild(ViewBase &child)
{
	child.setParent(this);
	children.push_back(&child);
}

void Views::ViewBase::setParent(ViewBase *parent)
{
	this->parent = parent;
}

void ViewBase::show(int mode)
{
	for (auto child : children) {
		child->show(mode);
	}

	if (!hwnd)
	{
		CreateAndRegisterClass();
		MakeWindow();
	}
	ShowWindow(hwnd, mode);
}

void ViewBase::set_hInstance(HINSTANCE hInstance)
{
	ViewBase::hInstance = hInstance;
}

void Views::ViewBase::onCreate()
{
}

void Views::ViewBase::onPaint(HDC hdc, PAINTSTRUCT * ps)
{
}

void Views::ViewBase::onDestroy()
{
	PostQuitMessage(0);
}

void ViewBase::CreateAndRegisterClass()
{
	WNDCLASS wc = { };
	wc.lpfnWndProc = Views::OnMessage;
	wc.hInstance = ViewBase::hInstance;
	wc.lpszClassName = className.c_str();

	RegisterClass(&wc);
}

void ViewBase::MakeWindow()
{
	hwnd = CreateWindowEx(
		0,
		className.c_str(),
		title.c_str(),
		style,
		x, y, width, height,
		parent ? parent->getHwnd() : nullptr,
		nullptr,
		ViewBase::hInstance,
		this
	);

	if (hwnd == NULL) {
		throw std::exception("Could not create window");
	}
}
