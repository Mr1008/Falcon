#pragma once
#include "pch.h"
#include "ControlEventsListener.h"
#include <vector>
#include <functional>
#include "../Falcon.Shared/Publisher.h"
#include "ResizeType.h"

namespace Controls
{
    class Control
        : public Shared::Publisher<ControlEventListener>
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

        void registerEventListener(ControlEventListener* listener);
        void unregisterEventListener(ControlEventListener* listener);

        virtual void onCreate() {};
        virtual void onCreated() {};
        virtual void onPaint() {};
        virtual void onDestroy() {};
        virtual void onResize(ResizeType type, const SIZE& size) {};
        virtual bool onMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) { return false; };

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

        int internalOnCreate();
        void internalOnCreated();
        int internalOnPaint();
        int internalOnDestroy();
        int internalOnResize(ResizeType type, const SIZE& size);
        int internalOnMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
    };
}