#pragma once
#include "Control.h"
#include "../Falcon.Shared/Publisher.h"
#include "TerminalRenderer.h"
#include <wrl.h>
#include <dxgi1_3.h>
#include <d3d11_2.h>
#include <d2d1_2.h>
#include <dcomp.h>
#include <mutex>
#pragma comment(lib, "dxgi")
#pragma comment(lib, "d3d11")
#pragma comment(lib, "d2d1")
#pragma comment(lib, "dcomp")

namespace Controls
{
    class DirectXWindow :
        public Control,
        private Shared::Publisher<TerminalRenderer>
    {
    public:
        DirectXWindow(
            const std::wstring& title,
            const std::wstring& windowClass,
            int x = CW_USEDEFAULT,
            int y = CW_USEDEFAULT,
            int width = CW_USEDEFAULT,
            int height = CW_USEDEFAULT,
            bool isAcrylicEnabled = false
        );

        virtual void onCreated();
        virtual void onDestroy();
        virtual void onResize(ResizeType type, const SIZE& size);
        virtual bool onMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

        void invalidate();
        void registerTerminalRenderer(TerminalRenderer* renderer);
        void unregisterTerminalRenderer(TerminalRenderer* renderer);
        void setTitle(const std::wstring& title);

    private:
        const size_t SWAP_BUFFERS = 2;
        const DXGI_FORMAT PIXEL_FORMAT = DXGI_FORMAT_B8G8R8A8_UNORM;

        enum DX_WINDOW_MSG {
            RENDER = WM_USER
        };

        std::mutex renderMutex;

        bool isAcrylicEnabled;
        Microsoft::WRL::ComPtr<IDXGIFactory2> dxgiFactory;
        Microsoft::WRL::ComPtr<IDXGIDevice> dxgiDevice;
        Microsoft::WRL::ComPtr<IDCompositionDevice> dCompDevice;
        Microsoft::WRL::ComPtr<IDXGISwapChain1> swapChain;
        Microsoft::WRL::ComPtr<ID2D1DeviceContext> dc;
        Microsoft::WRL::ComPtr<IDCompositionTarget> dCompTarget;
        Microsoft::WRL::ComPtr<IDCompositionVisual> dCompVisual;

        void internalRender(std::function<void(ID2D1DeviceContext*)>* fn);
        void createID2D1Bitmap();
        void notifyRenderers(std::function<void(TerminalRenderer*)> fn);
    };
}
