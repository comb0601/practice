/*
 * DirectX 11 Tutorial - Lesson 66
 * Example 03: Render Target View Creation
 *
 * Creates a complete rendering pipeline with render target view.
 */

#include <windows.h>
#include <d3d11.h>

#pragma comment(lib, "d3d11.lib")

IDXGISwapChain* g_swapChain = nullptr;
ID3D11Device* g_device = nullptr;
ID3D11DeviceContext* g_context = nullptr;
ID3D11RenderTargetView* g_rtv = nullptr;
bool g_running = true;

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (msg == WM_DESTROY || (msg == WM_KEYDOWN && wParam == VK_ESCAPE))
    {
        g_running = false;
        PostQuitMessage(0);
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

bool CreateRenderTarget()
{
    ID3D11Texture2D* backBuffer = nullptr;

    // Get back buffer from swap chain
    HRESULT hr = g_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
    if (FAILED(hr)) return false;

    // Create render target view
    hr = g_device->CreateRenderTargetView(backBuffer, nullptr, &g_rtv);
    backBuffer->Release();

    if (FAILED(hr)) return false;

    // Bind to output merger
    g_context->OMSetRenderTargets(1, &g_rtv, nullptr);

    return true;
}

bool InitDirectX(HWND hWnd, int width, int height)
{
    // Create device and swap chain
    DXGI_SWAP_CHAIN_DESC scd = {};
    scd.BufferCount = 1;
    scd.BufferDesc.Width = width;
    scd.BufferDesc.Height = height;
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scd.OutputWindow = hWnd;
    scd.SampleDesc.Count = 1;
    scd.Windowed = TRUE;

    HRESULT hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE,
        nullptr, 0, nullptr, 0, D3D11_SDK_VERSION, &scd,
        &g_swapChain, &g_device, nullptr, &g_context);

    if (FAILED(hr)) return false;

    // Create render target
    return CreateRenderTarget();
}

void Render()
{
    // Clear to cornflower blue
    float color[4] = { 0.39f, 0.58f, 0.93f, 1.0f };
    g_context->ClearRenderTargetView(g_rtv, color);

    // Present
    g_swapChain->Present(1, 0);
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int)
{
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0, 0,
                     hInst, nullptr, nullptr, nullptr, nullptr, L"DX11RTV", nullptr };
    RegisterClassEx(&wc);

    HWND hWnd = CreateWindow(wc.lpszClassName, L"Render Target View",
        WS_OVERLAPPEDWINDOW, 100, 100, 800, 600, nullptr, nullptr, hInst, nullptr);

    if (!InitDirectX(hWnd, 800, 600))
    {
        MessageBox(nullptr, L"DirectX init failed!", L"Error", MB_OK);
        return 1;
    }

    ShowWindow(hWnd, SW_SHOW);

    MSG msg;
    while (g_running)
    {
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        Render();
    }

    if (g_rtv) g_rtv->Release();
    if (g_context) g_context->Release();
    if (g_device) g_device->Release();
    if (g_swapChain) g_swapChain->Release();

    return 0;
}

/*
 * This example shows your first rendered frame!
 * The window will be filled with cornflower blue.
 */
