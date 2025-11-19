/*
 * Lesson 69 - HLSL-Basics
 * DirectX 11 Shader Entry Points
 *
 * Compilation: cl /EHsc filename.cpp d3d11.lib dxgi.lib user32.lib
 */

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <d3d11.h>
#include <dxgi.h>
#include <iostream>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "user32.lib")

// Global DirectX objects
ID3D11Device* g_device = nullptr;
ID3D11DeviceContext* g_context = nullptr;
IDXGISwapChain* g_swapChain = nullptr;
ID3D11RenderTargetView* g_renderTargetView = nullptr;

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

// Window procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        case WM_KEYDOWN:
            if (wParam == VK_ESCAPE) {
                PostQuitMessage(0);
            }
            return 0;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

bool InitDirectX(HWND hwnd) {
    // Create swap chain description
    DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
    swapChainDesc.BufferCount = 1;
    swapChainDesc.BufferDesc.Width = WINDOW_WIDTH;
    swapChainDesc.BufferDesc.Height = WINDOW_HEIGHT;
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
    swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.OutputWindow = hwnd;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.Windowed = TRUE;

    // Create device and swap chain
    HRESULT hr = D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        0,
        nullptr,
        0,
        D3D11_SDK_VERSION,
        &swapChainDesc,
        &g_swapChain,
        &g_device,
        nullptr,
        &g_context
    );

    if (FAILED(hr)) {
        std::cerr << "Failed to create D3D11 device and swap chain\n";
        return false;
    }

    // Create render target view
    ID3D11Texture2D* backBuffer = nullptr;
    g_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
    g_device->CreateRenderTargetView(backBuffer, nullptr, &g_renderTargetView);
    backBuffer->Release();

    // Set render target
    g_context->OMSetRenderTargets(1, &g_renderTargetView, nullptr);

    // Set viewport
    D3D11_VIEWPORT viewport = {};
    viewport.Width = (float)WINDOW_WIDTH;
    viewport.Height = (float)WINDOW_HEIGHT;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    g_context->RSSetViewports(1, &viewport);

    std::cout << "DirectX 11 initialized successfully\n";
    std::cout << "Lesson 69: Shader Entry Points\n";
    return true;
}

void Render() {
    // Clear the back buffer
    float clearColor[4] = { 0.1f, 0.2f, 0.4f, 1.0f };  // Blue background
    g_context->ClearRenderTargetView(g_renderTargetView, clearColor);

    // Present the back buffer
    g_swapChain->Present(0, 0);
}

void Cleanup() {
    if (g_renderTargetView) g_renderTargetView->Release();
    if (g_swapChain) g_swapChain->Release();
    if (g_context) g_context->Release();
    if (g_device) g_device->Release();
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int) {
    // Register window class
    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "DirectXWindow";
    RegisterClass(&wc);

    // Create window
    HWND hwnd = CreateWindow(
        "DirectXWindow", "Lesson 69: Shader Entry Points",
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
        WINDOW_WIDTH, WINDOW_HEIGHT, nullptr, nullptr, hInstance, nullptr
    );

    if (!hwnd) {
        std::cerr << "Failed to create window\n";
        return 1;
    }

    ShowWindow(hwnd, SW_SHOW);

    // Initialize DirectX
    if (!InitDirectX(hwnd)) {
        return 1;
    }

    // Main message loop
    MSG msg = {};
    while (msg.message != WM_QUIT) {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        } else {
            Render();
        }
    }

    Cleanup();
    return 0;
}
