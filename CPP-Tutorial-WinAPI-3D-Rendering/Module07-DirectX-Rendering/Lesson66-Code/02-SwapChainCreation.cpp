/*
 * DirectX 11 Tutorial - Lesson 66
 * Example 02: Swap Chain Creation
 *
 * Complete window creation and swap chain initialization.
 */

#include <windows.h>
#include <d3d11.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

IDXGISwapChain* g_swapChain = nullptr;
ID3D11Device* g_device = nullptr;
ID3D11DeviceContext* g_context = nullptr;
bool g_running = true;

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (msg == WM_DESTROY || (msg == WM_KEYDOWN && wParam == VK_ESCAPE))
    {
        g_running = false;
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

bool InitDirectX(HWND hWnd, int width, int height)
{
    DXGI_SWAP_CHAIN_DESC scd = {};
    scd.BufferCount = 1;
    scd.BufferDesc.Width = width;
    scd.BufferDesc.Height = height;
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    scd.BufferDesc.RefreshRate.Numerator = 60;
    scd.BufferDesc.RefreshRate.Denominator = 1;
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scd.OutputWindow = hWnd;
    scd.SampleDesc.Count = 1;
    scd.Windowed = TRUE;

    HRESULT hr = D3D11CreateDeviceAndSwapChain(
        nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0,
        nullptr, 0, D3D11_SDK_VERSION, &scd,
        &g_swapChain, &g_device, nullptr, &g_context
    );

    return SUCCEEDED(hr);
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int)
{
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0, 0,
                     hInst, nullptr, nullptr, nullptr, nullptr,
                     L"DX11SwapChain", nullptr };
    RegisterClassEx(&wc);

    HWND hWnd = CreateWindow(wc.lpszClassName, L"Swap Chain Creation",
                            WS_OVERLAPPEDWINDOW, 100, 100, 800, 600,
                            nullptr, nullptr, hInst, nullptr);

    if (!InitDirectX(hWnd, 800, 600))
    {
        MessageBox(nullptr, L"DirectX initialization failed!", L"Error", MB_OK);
        return 1;
    }

    ShowWindow(hWnd, SW_SHOWDEFAULT);

    MSG msg;
    while (g_running)
    {
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        // Render (just present for now)
        g_swapChain->Present(1, 0);
    }

    if (g_context) g_context->Release();
    if (g_device) g_device->Release();
    if (g_swapChain) g_swapChain->Release();

    return 0;
}
