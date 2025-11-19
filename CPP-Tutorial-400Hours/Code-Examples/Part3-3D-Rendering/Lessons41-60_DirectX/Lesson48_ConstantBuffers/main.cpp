#ifdef _WIN32

#include <iostream>
#include <windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include "../../Common/Math3D/Math3D.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

ID3D11Device* g_pd3dDevice = nullptr;
ID3D11DeviceContext* g_pImmediateContext = nullptr;
IDXGISwapChain* g_pSwapChain = nullptr;
ID3D11RenderTargetView* g_pRenderTargetView = nullptr;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}

int main() {
    std::cout << "Constant Buffer Implementation" << std::endl;
    std::cout << "DirectX 11 Application" << std::endl;
    std::cout << "Press ESC to exit" << std::endl;

    // Window creation
    WNDCLASSEX wcex = {};
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.hInstance = GetModuleHandle(nullptr);
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.lpszClassName = L"DirectXLessonClass";
    RegisterClassEx(&wcex);

    HWND hWnd = CreateWindow(L"DirectXLessonClass", L"Constant Buffer Implementation_W",
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
        WINDOW_WIDTH, WINDOW_HEIGHT, nullptr, nullptr,
        GetModuleHandle(nullptr), nullptr);

    if (!hWnd) {
        std::cerr << "Failed to create window" << std::endl;
        return -1;
    }

    ShowWindow(hWnd, SW_SHOW);

    // Initialize DirectX
    DXGI_SWAP_CHAIN_DESC sd = {};
    sd.BufferCount = 1;
    sd.BufferDesc.Width = WINDOW_WIDTH;
    sd.BufferDesc.Height = WINDOW_HEIGHT;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;

    D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;

    HRESULT hr = D3D11CreateDeviceAndSwapChain(
        nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0,
        &featureLevel, 1, D3D11_SDK_VERSION, &sd,
        &g_pSwapChain, &g_pd3dDevice, nullptr, &g_pImmediateContext
    );

    if (FAILED(hr)) {
        std::cerr << "Failed to create DirectX device and swap chain" << std::endl;
        return -1;
    }

    // Create render target view
    ID3D11Texture2D* pBackBuffer = nullptr;
    g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer);
    g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_pRenderTargetView);
    pBackBuffer->Release();

    g_pImmediateContext->OMSetRenderTargets(1, &g_pRenderTargetView, nullptr);

    // Viewport
    D3D11_VIEWPORT vp = {};
    vp.Width = (FLOAT)WINDOW_WIDTH;
    vp.Height = (FLOAT)WINDOW_HEIGHT;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    g_pImmediateContext->RSSetViewports(1, &vp);

    std::cout << "DirectX initialized successfully!" << std::endl;

    // Main loop
    MSG msg = {};
    while (msg.message != WM_QUIT) {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        } else {
            // Render
            float clearColor[4] = { 0.2f, 0.3f, 0.4f, 1.0f };
            g_pImmediateContext->ClearRenderTargetView(g_pRenderTargetView, clearColor);

            // TODO: Add rendering code here

            g_pSwapChain->Present(0, 0);
        }
    }

    // Cleanup
    if (g_pRenderTargetView) g_pRenderTargetView->Release();
    if (g_pSwapChain) g_pSwapChain->Release();
    if (g_pImmediateContext) g_pImmediateContext->Release();
    if (g_pd3dDevice) g_pd3dDevice->Release();

    return 0;
}

#else
#include <iostream>
int main() {
    std::cout << "DirectX lessons are only available on Windows" << std::endl;
    return 0;
}
#endif
