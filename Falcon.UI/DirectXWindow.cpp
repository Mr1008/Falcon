#include "pch.h"
#include "DirectXWindow.h"
#include "../Falcon.Shared/COMUtils.h"

enum AccentState
{
    ACCENT_DISABLED = 0,
    ACCENT_ENABLE_GRADIENT = 1,
    ACCENT_ENABLE_TRANSPARENTGRADIENT = 2,
    ACCENT_ENABLE_BLURBEHIND = 3,
    ACCENT_ENABLE_ACRYLICBLURBEHIND = 4,
    ACCENT_INVALID_STATE = 5
};

struct ACCENTPOLICY
{
    int nAccentState;
    int nFlags;
    int nColor;
    int nAnimationId;
};
struct WINCOMPATTRDATA
{
    int nAttribute;
    PVOID pData;
    ULONG ulDataSize;
};

namespace Controls
{
    using namespace std;
    using namespace Microsoft::WRL;

    DirectXWindow::DirectXWindow(
        const std::wstring& title,
        const std::wstring& windowClass,
        int x, int y, int width, int height,
        bool isAcrylicEnabled) :
        Control(windowClass, title, WS_OVERLAPPEDWINDOW, WS_EX_NOREDIRECTIONBITMAP, x, y, width, height),
        isAcrylicEnabled(isAcrylicEnabled)
    {
    }

    bool DirectXWindow::onMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        switch (uMsg) {
        case DX_WINDOW_MSG::RENDER:
            internalRender(reinterpret_cast<function<void(ID2D1DeviceContext*)>*>(wParam));
            return true;
        default:
            return false;
        }
    }

    void DirectXWindow::onCreated()
    {
        if (isAcrylicEnabled)
        {
            const HINSTANCE hModule = LoadLibrary(TEXT("user32.dll"));
            typedef BOOL(WINAPI * pSetWindowCompositionAttribute)(HWND, WINCOMPATTRDATA*);
            const pSetWindowCompositionAttribute SetWindowCompositionAttribute = (pSetWindowCompositionAttribute)GetProcAddress(hModule, "SetWindowCompositionAttribute");
            ACCENTPOLICY accent = {};
            accent.nAccentState = AccentState::ACCENT_ENABLE_ACRYLICBLURBEHIND;
            accent.nColor = (2 << 24) | (0x990000 & 0xFFFFFF);
            WINCOMPATTRDATA d = {};
            d.nAttribute = 19;
            d.pData = &accent;
            d.ulDataSize = sizeof(accent);
            SetWindowCompositionAttribute(getHwnd(), &d);
        }

        D3D_FEATURE_LEVEL featureLevel;
        ComPtr<ID3D11Device> d3d11Device;
        unsigned int d3d11CreateDeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#ifdef _DEBUG
        d3d11CreateDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
        HR(D3D11CreateDevice(
            nullptr,
            D3D_DRIVER_TYPE_HARDWARE,
            nullptr,
            d3d11CreateDeviceFlags,
            nullptr,
            0,
            D3D11_SDK_VERSION,
            &d3d11Device,
            &featureLevel,
            nullptr
        ));

        HR(d3d11Device.As(&dxgiDevice));

        HR(CreateDXGIFactory2(
            DXGI_CREATE_FACTORY_DEBUG,
            __uuidof(dxgiFactory),
            reinterpret_cast<void**>(dxgiFactory.GetAddressOf())
        ));


        D2D1_FACTORY_OPTIONS d2d1FactoryOptions = { };
#ifdef _DEBUG
        d2d1FactoryOptions.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
#endif

        ComPtr<ID2D1Factory2> d2d1Factory;
        HR(D2D1CreateFactory(
            D2D1_FACTORY_TYPE_SINGLE_THREADED,
            d2d1FactoryOptions,
            d2d1Factory.GetAddressOf()
        ));

        ComPtr<ID2D1Device1> d2d1Device;
        HR(d2d1Factory->CreateDevice(
            dxgiDevice.Get(),
            d2d1Device.GetAddressOf()
        ));

        HR(d2d1Device->CreateDeviceContext(
            D2D1_DEVICE_CONTEXT_OPTIONS_NONE,
            dc.GetAddressOf()
        ));

        HR(DCompositionCreateDevice(
            dxgiDevice.Get(),
            __uuidof(dCompDevice),
            reinterpret_cast<void**>(dCompDevice.GetAddressOf())
        ));

        HR(dCompDevice->CreateTargetForHwnd(
            getHwnd(),
            true,
            dCompTarget.GetAddressOf()
        ));

        HR(dCompDevice->CreateVisual(
            dCompVisual.GetAddressOf()
        ));

        DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
        swapChainDesc.Format = PIXEL_FORMAT;
        swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
        swapChainDesc.BufferCount = SWAP_BUFFERS;
        swapChainDesc.SampleDesc.Count = 1;
        swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_PREMULTIPLIED;
        RECT rect = getClientRect();
        swapChainDesc.Width = rect.right - rect.left;
        swapChainDesc.Height = rect.bottom - rect.top;

        HR(dxgiFactory->CreateSwapChainForComposition(
            dxgiDevice.Get(),
            &swapChainDesc,
            nullptr,
            swapChain.GetAddressOf()
        ));

        createID2D1Bitmap();

        HR(dCompTarget->SetRoot(
            dCompVisual.Get()
        ));

        notifyRenderers([this](TerminalRenderer* r) {r->onCreateDxResources(dc.Get()); });
    }

    void DirectXWindow::onDestroy()
    {
        notifyRenderers([](TerminalRenderer* r) {r->onReleaseDxResources(); });
    }

    void DirectXWindow::onResize(ResizeType type, const SIZE& size)
    {
        if (type == ResizeType::Restored || type == ResizeType::Maximized)
        {
            dc->SetTarget(nullptr);
            HR(swapChain->ResizeBuffers(
                SWAP_BUFFERS,
                size.cx > 0 ? size.cx : 1,
                size.cy > 0 ? size.cy : 1,
                PIXEL_FORMAT,
                0
            ));
            createID2D1Bitmap();
            notifyRenderers([&](TerminalRenderer* r) {r->onResizeScene(type, size); });
        }
        Control::onResize(type, size);
    }

    void DirectXWindow::invalidate()
    {
        SendMessage(getHwnd(), DX_WINDOW_MSG::RENDER, reinterpret_cast<WPARAM>(&function([this](ID2D1DeviceContext* dc) {
            notifyRenderers([&](TerminalRenderer* r) {
                r->onRender(dc);
                });
            })), 0);
    }

    void DirectXWindow::internalRender(function<void(ID2D1DeviceContext*)>* fn)
    {
        renderMutex.lock();
        dc->BeginDraw();
        fn->operator()(dc.Get());
        HR(dc->EndDraw());
        HR(swapChain->Present(1, 0));
        HR(dCompDevice->Commit());
        renderMutex.unlock();
    }

    void DirectXWindow::createID2D1Bitmap()
    {
        ComPtr<IDXGISurface2> dxgiSurface;
        HR(swapChain->GetBuffer(
            0,
            __uuidof(dxgiSurface),
            reinterpret_cast<void**>(dxgiSurface.GetAddressOf())
        ));

        D2D1_BITMAP_PROPERTIES1 bitmapProperties = {};
        bitmapProperties.pixelFormat.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED;
        bitmapProperties.pixelFormat.format = DXGI_FORMAT_B8G8R8A8_UNORM;
        bitmapProperties.bitmapOptions = D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW;

        ComPtr<ID2D1Bitmap1> d2d1Bitmap;
        HR(dc->CreateBitmapFromDxgiSurface(
            dxgiSurface.Get(),
            bitmapProperties,
            d2d1Bitmap.GetAddressOf()
        ));
        dc->SetTarget(d2d1Bitmap.Get());
        HR(dCompVisual->SetContent(
            swapChain.Get()
        ));
    }

    void DirectXWindow::notifyRenderers(function<void(TerminalRenderer*)> fn)
    {
        Publisher<TerminalRenderer>::notifyListeners(fn);
    }

    void DirectXWindow::registerTerminalRenderer(TerminalRenderer* renderer)
    {
        Publisher<TerminalRenderer>::registerListener(renderer);
    }

    void DirectXWindow::unregisterTerminalRenderer(TerminalRenderer* renderer)
    {
        Publisher<TerminalRenderer>::unregisterListener(renderer);
    }

    void DirectXWindow::setTitle(const std::wstring& title)
    {
        SetWindowText(getHwnd(), title.c_str());
    }
}
