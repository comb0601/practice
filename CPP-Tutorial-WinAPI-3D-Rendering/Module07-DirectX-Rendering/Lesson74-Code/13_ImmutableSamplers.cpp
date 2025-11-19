/*
 * Lesson 74: Sampler-States
 * Topic: ImmutableSamplers
 *
 * DirectX 11 implementation with full Windows integration
 *
 * Compilation:
 * cl /EHsc ImmutableSamplers.cpp d3d11.lib dxgi.lib d3dcompiler.lib user32.lib
 */

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <iostream>
#include <string>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

// Global DirectX objects
ID3D11Device* g_device = nullptr;
ID3D11DeviceContext* g_context = nullptr;
IDXGISwapChain* g_swapChain = nullptr;
ID3D11RenderTargetView* g_renderTargetView = nullptr;
ID3D11DepthStencilView* g_depthStencilView = nullptr;

const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (msg == WM_DESTROY || (msg == WM_KEYDOWN && wParam == VK_ESCAPE)) {
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

bool InitDirectX(HWND hwnd) {
    DXGI_SWAP_CHAIN_DESC scd = {};
    scd.BufferCount = 1;
    scd.BufferDesc.Width = WINDOW_WIDTH;
    scd.BufferDesc.Height = WINDOW_HEIGHT;
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scd.OutputWindow = hwnd;
    scd.SampleDesc.Count = 1;
    scd.Windowed = TRUE;

    D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0,
                                  nullptr, 0, D3D11_SDK_VERSION, &scd,
                                  &g_swapChain, &g_device, nullptr, &g_context);

    ID3D11Texture2D* backBuffer;
    g_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
    g_device->CreateRenderTargetView(backBuffer, nullptr, &g_renderTargetView);
    backBuffer->Release();

    D3D11_VIEWPORT vp = {0, 0, (float)WINDOW_WIDTH, (float)WINDOW_HEIGHT, 0, 1};
    g_context->RSSetViewports(1, &vp);
    g_context->OMSetRenderTargets(1, &g_renderTargetView, nullptr);

    std::cout << "DirectX 11 Initialized\n";
    std::cout << "Lesson " << 74 << ": " << "ImmutableSamplers" << "\n";
    return true;
}

void Render() {
    float color[4] = {0.0f, 0.2f, 0.4f, 1.0f};
    g_context->ClearRenderTargetView(g_renderTargetView, color);

    // Topic-specific rendering code goes here

    g_swapChain->Present(1, 0);
}

void Cleanup() {
    if (g_depthStencilView) g_depthStencilView->Release();
    if (g_renderTargetView) g_renderTargetView->Release();
    if (g_swapChain) g_swapChain->Release();
    if (g_context) g_context->Release();
    if (g_device) g_device->Release();
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int) {
    WNDCLASS wc = {0, WndProc, 0, 0, hInst, 0, 0, 0, 0, "D3DWin"};
    RegisterClass(&wc);
    HWND hwnd = CreateWindow("D3DWin", "Lesson 74: ImmutableSamplers",
                             WS_OVERLAPPEDWINDOW, 100, 100,
                             WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0, hInst, 0);
    ShowWindow(hwnd, SW_SHOW);

    if (!InitDirectX(hwnd)) return 1;

    MSG msg = {};
    while (msg.message != WM_QUIT) {
        if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        } else {
            Render();
        }
    }

    Cleanup();
    return 0;
}
