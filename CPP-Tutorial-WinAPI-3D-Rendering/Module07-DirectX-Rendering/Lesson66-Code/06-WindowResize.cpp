/*
 * DirectX 11 - Lesson 66
 * Example 06: Handling Window Resize
 *
 * Properly handles window resizing by recreating render targets.
 */

#include <windows.h>
#include <d3d11.h>
#include <sstream>

#pragma comment(lib, "d3d11.lib")

IDXGISwapChain* g_swapChain = nullptr;
ID3D11Device* g_device = nullptr;
ID3D11DeviceContext* g_context = nullptr;
ID3D11RenderTargetView* g_rtv = nullptr;

int g_width = 800;
int g_height = 600;

void CleanupRenderTarget()
{
    if (g_rtv)
    {
        g_rtv->Release();
        g_rtv = nullptr;
    }
}

bool CreateRenderTarget()
{
    ID3D11Texture2D* backBuffer;
    g_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
    g_device->CreateRenderTargetView(backBuffer, nullptr, &g_rtv);
    backBuffer->Release();

    g_context->OMSetRenderTargets(1, &g_rtv, nullptr);

    D3D11_VIEWPORT vp = {};
    vp.Width = (float)g_width;
    vp.Height = (float)g_height;
    vp.MaxDepth = 1.0f;
    g_context->RSSetViewports(1, &vp);

    return true;
}

void OnResize(int width, int height)
{
    if (!g_swapChain) return;

    g_width = width;
    g_height = height;

    CleanupRenderTarget();
    g_swapChain->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, 0);
    CreateRenderTarget();
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_SIZE:
        if (wParam != SIZE_MINIMIZED)
        {
            OnResize(LOWORD(lParam), HIWORD(lParam));
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

bool InitDirectX(HWND hWnd)
{
    DXGI_SWAP_CHAIN_DESC scd = {};
    scd.BufferCount = 1;
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scd.OutputWindow = hWnd;
    scd.SampleDesc.Count = 1;
    scd.Windowed = TRUE;

    D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0,
        nullptr, 0, D3D11_SDK_VERSION, &scd, &g_swapChain, &g_device, nullptr, &g_context);

    return CreateRenderTarget();
}

void Render()
{
    // Color changes based on aspect ratio
    float aspectRatio = (float)g_width / g_height;
    float color[4] = { aspectRatio / 2.0f, 0.2f, 0.4f, 1.0f };

    g_context->ClearRenderTargetView(g_rtv, color);
    g_swapChain->Present(1, 0);
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int)
{
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), 0, WndProc, 0, 0, hInst, 0, 0, 0, 0, L"DX11Resize", 0 };
    RegisterClassEx(&wc);
    HWND hWnd = CreateWindow(L"DX11Resize", L"Window Resize - Try resizing!",
        WS_OVERLAPPEDWINDOW, 100, 100, g_width, g_height, 0, 0, hInst, 0);

    InitDirectX(hWnd);
    ShowWindow(hWnd, SW_SHOW);

    MSG msg = {};
    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else Render();
    }

    CleanupRenderTarget();
    if (g_context) g_context->Release();
    if (g_device) g_device->Release();
    if (g_swapChain) g_swapChain->Release();

    return 0;
}

/*
 * KEY POINTS:
 * - Always release render target before ResizeBuffers()
 * - Recreate render target after resize
 * - Update viewport to match new size
 * - Handle SIZE_MINIMIZED to avoid 0x0 resize
 */
