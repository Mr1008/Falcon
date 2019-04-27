#include "pch.h"
#include "Control.h"
#include "Mouse.h"

namespace Controls
{
	using namespace std;
	HINSTANCE Control::hInstance;

	LRESULT CALLBACK OnMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		if (uMsg == WM_CREATE) {
			CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
			Control* view = reinterpret_cast<Control*>(pCreate->lpCreateParams);
			SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(view));
			return view->internalOnCreate();
		}

		Control* view = reinterpret_cast<Control*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
		if (view != nullptr) {
			return view->internalOnMessage(uMsg, wParam, lParam);
		}

		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	Control::Control(const std::wstring& className, const std::wstring& title, DWORD style, DWORD exStyle, int x, int y, int width, int height) :
		className(className),
		title(title),
		style(style),
		exStyle(exStyle),
		pos({ x, y }),
		size({ width, height }),
		defaultChild(nullptr),
		parent(nullptr),
		supportsGDIPainting(false)
	{
	}

	void Control::addChild(Control& child, bool defaultFocus)
	{
		child.setParent(this);
		children.push_back(&child);
		defaultChild = &child;
	}

	void Control::setParent(Control* parent)
	{
		this->parent = parent;
	}

	void Control::show(int mode)
	{
		if (hwnd == nullptr) {
			createAndRegisterClass();
			createControl();
		}

		if (hwnd == nullptr) {
			throw exception("Window in incorrect state.");
		}

		ShowWindow(hwnd, mode);

		for (auto child : children) {
			child->show(mode);
		}

		if (defaultChild != nullptr) {
			defaultChild->focus();
		}
	}

	void Control::close()
	{
		SendMessage(hwnd, WM_DESTROY, 0, 0);
	}

	void Control::focus()
	{
		SetFocus(hwnd);
	}

	bool Control::hasFocus()
	{
		return GetFocus() == hwnd;
	}

	void Control::set_hInstance(HINSTANCE hInstance)
	{
		Control::hInstance = hInstance;
	}

	void Control::registerEventListener(ControlEventListener* listener)
	{
		registerListener(listener);
	}

	void Control::unregisterEventListener(ControlEventListener* listener)
	{
		unregisterListener(listener);
	}

	int Control::internalOnCreate()
	{
		onCreate();
		return 0;
	}

	void Control::internalOnCreated()
	{
		onCreated();
	}

	int Control::internalOnPaint()
	{
		onPaint();
		return 0;
	}

	int Control::internalOnDestroy()
	{
		onDestroy();
		return 0;
	}

	int Control::internalOnResize(ResizeType reason, const SIZE& size)
	{
		this->size = size;
		onResize(reason, size);
		notifyListeners([&](ControlEventListener * listener) {listener->onSizeChanged(reason, size); });
		return 0;
	}

	int Control::internalOnMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
		case WM_DESTROY:
			return internalOnDestroy();
		case WM_PAINT:
		{
			if (supportsGDIPainting)
				return internalOnPaint();
			else
				break;
		}
		case WM_SIZE:
		{
			ResizeType type = ResizeType::Unknown;
			switch (wParam)
			{
			case SIZE_RESTORED:
				type = ResizeType::Restored;
				break;
			case SIZE_MINIMIZED:
				type = ResizeType::Minimized;
				break;
			case SIZE_MAXSHOW:
				type = ResizeType::MaxShow;
				break;
			case SIZE_MAXIMIZED:
				type = ResizeType::Maximized;
				break;
			case SIZE_MAXHIDE:
				type = ResizeType::MaxHide;
				break;
			}
			return internalOnResize(type, { LOWORD(lParam), HIWORD(lParam) });
		}
		case WM_MOUSEMOVE:
		{
			POINT pos = { LOWORD(lParam), HIWORD(lParam) };
			notifyListeners([&pos](InputListener * l) {
				l->onMouseMoved(pos);
				});
			return 0;
		}
		case WM_LBUTTONDOWN:
		{
			POINT pos = { LOWORD(lParam), HIWORD(lParam) };
			notifyListeners([&pos](InputListener * l) {
				l->onMouseButtonDown(pos, MouseButton::Left);
				});
			return 0;
		}
		case WM_LBUTTONUP:
		{
			POINT pos = { LOWORD(lParam), HIWORD(lParam) };
			notifyListeners([&pos](InputListener * l) {
				l->onMouseButtonUp(pos, MouseButton::Left);
				});
			return 0;
		}
		case WM_MBUTTONDOWN:
		{
			POINT pos = { LOWORD(lParam), HIWORD(lParam) };
			notifyListeners([&pos](InputListener * l) {
				l->onMouseButtonDown(pos, MouseButton::Middle);
				});
			return 0;
		}
		case WM_MBUTTONUP:
		{
			POINT pos = { LOWORD(lParam), HIWORD(lParam) };
			notifyListeners([&pos](InputListener * l) {
				l->onMouseButtonUp(pos, MouseButton::Middle);
				});
			return 0;
		}
		case WM_RBUTTONDOWN:
		{
			POINT pos = { LOWORD(lParam), HIWORD(lParam) };
			notifyListeners([&pos](InputListener * l) {
				l->onMouseButtonDown(pos, MouseButton::Right);
				});
			return 0;
		}
		case WM_RBUTTONUP:
		{
			POINT pos = { LOWORD(lParam), HIWORD(lParam) };
			notifyListeners([&pos](InputListener * l) {
				l->onMouseButtonUp(pos, MouseButton::Right);
				});
			return 0;
		}
		case WM_CHAR:
		{
			wchar_t key = static_cast<wchar_t>(wParam);
			bool firstOccurence = !(lParam & 1 << 30);
			unsigned int repeatCount = lParam & 0xffff;
			notifyListeners([key, firstOccurence, repeatCount](InputListener * l) {
				l->onKeyPushed(key, firstOccurence, repeatCount);
				});
			return 0;
		}
		}

		if (onMessage(uMsg, wParam, lParam)) {
			return 0;
		}

		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	void Control::setControlPosAndSize(POINT * pos, const SIZE * size)
	{
		if (pos) {
			this->pos = *pos;
		}
		if (size) {
			this->size = *size;
		}

		SetWindowPos(
			hwnd,
			nullptr,
			pos ? pos->x : 0,
			pos ? pos->y : 0,
			size ? size->cx : 0,
			size ? size->cy : 0,
			(!size ? SWP_NOSIZE : 0) | (!pos ? SWP_NOMOVE : 0) | SWP_NOZORDER | SWP_SHOWWINDOW
		);
	}

	void Control::forceRender(const RECT * rect)
	{
		InvalidateRect(hwnd, rect, false);
	}

	const SIZE Control::getSize() const
	{
		return size;
	}

	const RECT Control::getClientRect() const
	{
		RECT rect = {};
		GetClientRect(hwnd, &rect);
		return rect;
	}

	const POINT Control::getPos() const
	{
		return pos;
	}

	void Control::createAndRegisterClass()
	{
		WNDCLASS wc = { };
		wc.lpfnWndProc = Controls::OnMessage;
		wc.hInstance = Control::hInstance;
		wc.lpszClassName = className.c_str();

		RegisterClass(&wc);
	}

	void Control::createControl()
	{
		if (pos.x == CW_USEDEFAULT) pos.x = 0;
		if (pos.y == CW_USEDEFAULT) pos.y = 0;
		if (parent) {
			if (size.cx == CW_DEFAULT) size.cx = parent->size.cx;
			if (size.cy == CW_DEFAULT) size.cy = parent->size.cy;
		}
		hwnd = CreateWindowEx(
			exStyle,
			className.c_str(),
			title.c_str(),
			style,
			pos.x, pos.y, size.cx, size.cy,
			parent ? parent->getHwnd() : nullptr,
			nullptr,
			Control::hInstance,
			this
		);

		if (hwnd == nullptr)
			throw std::exception("Could not create window");

		internalOnCreated();
	}
}
