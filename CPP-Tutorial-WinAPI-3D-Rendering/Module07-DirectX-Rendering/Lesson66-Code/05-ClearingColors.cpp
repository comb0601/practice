/*
 * DirectX 11 - Lesson 66
 * Example 05: Clearing with Different Colors
 *
 * Demonstrates clearing the screen with various colors.
 * Press SPACE to cycle through colors.
 */

#include <windows.h>
#include <d3d11.h>

#pragma comment(lib, "d3d11.lib")

IDXGISwapChain* g_swapChain = nullptr;
ID3D11Device* g_device = nullptr;
ID3D11DeviceContext* g_context = nullptr;
ID3D11RenderTargetView* g_rtv = nullptr;

int g_colorIndex = 0;

float g_colors[][4] = {
    { 0.39f, 0.58f, 0.93f, 1.0f },  // Cornflower blue
    { 1.0f, 0.0f, 0.0f, 1.0f },     // Red
    { 0.0f, 1.0f, 0.0f, 1.0f },     // Green
    { 0.0f, 0.0f, 1.0f, 1.0f },     // Blue
    { 1.0f, 1.0f, 0.0f, 1.0f },     // Yellow
    { 1.0f, 0.0f, 1.0f, 1.0f },     // Magenta
    { 0.0f, 1.0f, 1.0f, 1.0f },     // Cyan
    { 0.0f, 0.0f, 0.0f, 1.0f },     // Black
    { 1.0f, 1.0f, 1.0f, 1.0f },     // White
};

const int g_numColors = sizeof(g_colors) / sizeof(g_colors[0]);

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_KEYDOWN:
        if (wParam == VK_SPACE)
        {
            g_colorIndex = (g_colorIndex + 1) % g_numColors;
        }
        else if (wParam == VK_ESCAPE)
        {
            PostQuitMessage(0);
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

    ID3D11Texture2D* backBuffer;
    g_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
    g_device->CreateRenderTargetView(backBuffer, nullptr, &g_rtv);
    backBuffer->Release();

    g_context->OMSetRenderTargets(1, &g_rtv, nullptr);

    D3D11_VIEWPORT vp = {};
    vp.Width = 800; vp.Height = 600; vp.MaxDepth = 1.0f;
    g_context->RSSetViewports(1, &vp);

    return true;
}

void Render()
{
    g_context->ClearRenderTargetView(g_rtv, g_colors[g_colorIndex]);
    g_swapChain->Present(1, 0);
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int)
{
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), 0, WndProc, 0, 0, hInst, 0, 0, 0, 0, L"DX11Clear", 0 };
    RegisterClassEx(&wc);
    HWND hWnd = CreateWindow(L"DX11Clear", L"Clear Colors - Press SPACE",
        WS_OVERLAPPEDWINDOW, 100, 100, 800, 600, 0, 0, hInst, 0);

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

    if (g_rtv) g_rtv->Release();
    if (g_context) g_context->Release();
    if (g_device) g_device->Release();
    if (g_swapChain) g_swapChain->Release();

    return 0;
}
