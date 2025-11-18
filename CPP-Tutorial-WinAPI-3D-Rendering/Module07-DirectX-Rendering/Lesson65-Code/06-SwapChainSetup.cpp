/*
 * DirectX 11 Tutorial - Lesson 65
 * Example 06: Swap Chain Setup
 *
 * Creates a complete DirectX window with swap chain.
 * This is the foundation for all DirectX rendering.
 */

#include <windows.h>
#include <d3d11.h>
#include <dxgi.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

// Global DirectX objects
IDXGISwapChain* g_swapChain = nullptr;
ID3D11Device* g_device = nullptr;
ID3D11DeviceContext* g_context = nullptr;
ID3D11RenderTargetView* g_renderTargetView = nullptr;

HWND g_hWnd = nullptr;
bool g_isRunning = true;

// Window procedure
LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_DESTROY:
        g_isRunning = false;
        PostQuitMessage(0);
        return 0;

    case WM_KEYDOWN:
        if (wParam == VK_ESCAPE) PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

// Initialize DirectX
bool InitDirectX(HWND hWnd, int width, int height)
{
    // Describe swap chain
    DXGI_SWAP_CHAIN_DESC scd = { 0 };
    scd.BufferCount = 1;                                    // One back buffer
    scd.BufferDesc.Width = width;
    scd.BufferDesc.Height = height;
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;    // 32-bit color
    scd.BufferDesc.RefreshRate.Numerator = 60;
    scd.BufferDesc.RefreshRate.Denominator = 1;
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scd.OutputWindow = hWnd;
    scd.SampleDesc.Count = 1;                               // No multisampling
    scd.SampleDesc.Quality = 0;
    scd.Windowed = TRUE;
    scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    // Create device and swap chain
    D3D_FEATURE_LEVEL featureLevel;
    HRESULT hr = D3D11CreateDeviceAndSwapChain(
        nullptr,                        // Use default adapter
        D3D_DRIVER_TYPE_HARDWARE,       // Use hardware rendering
        nullptr,                        // No software module
        0,                              // No flags
        nullptr,                        // Default feature levels
        0,
        D3D11_SDK_VERSION,
        &scd,
        &g_swapChain,
        &g_device,
        &featureLevel,
        &g_context
    );

    if (FAILED(hr))
    {
        MessageBox(hWnd, L"Failed to create Direct3D device and swap chain", L"Error", MB_OK);
        return false;
    }

    // Get back buffer
    ID3D11Texture2D* backBuffer = nullptr;
    g_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);

    // Create render target view
    g_device->CreateRenderTargetView(backBuffer, nullptr, &g_renderTargetView);
    backBuffer->Release();

    // Set render target
    g_context->OMSetRenderTargets(1, &g_renderTargetView, nullptr);

    // Set viewport
    D3D11_VIEWPORT viewport = { 0 };
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = static_cast<float>(width);
    viewport.Height = static_cast<float>(height);
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;

    g_context->RSSetViewports(1, &viewport);

    return true;
}

// Clean up DirectX
void CleanupDirectX()
{
    if (g_renderTargetView) g_renderTargetView->Release();
    if (g_swapChain) g_swapChain->Release();
    if (g_context) g_context->Release();
    if (g_device) g_device->Release();
}

// Render frame
void Render()
{
    // Clear screen to cornflower blue
    float color[4] = { 0.39f, 0.58f, 0.93f, 1.0f };
    g_context->ClearRenderTargetView(g_renderTargetView, color);

    // Present
    g_swapChain->Present(1, 0);  // VSync on
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
    // Register window class
    WNDCLASSEX wc = { sizeof(WNDCLASSEX) };
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"DX11SwapChain";
    RegisterClassEx(&wc);

    // Create window
    const int WIDTH = 800, HEIGHT = 600;
    RECT rect = { 0, 0, WIDTH, HEIGHT };
    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

    g_hWnd = CreateWindowEx(0, wc.lpszClassName, L"DirectX 11 Swap Chain",
                           WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
                           rect.right - rect.left, rect.bottom - rect.top,
                           nullptr, nullptr, hInstance, nullptr);

    if (!g_hWnd || !InitDirectX(g_hWnd, WIDTH, HEIGHT))
    {
        return 1;
    }

    ShowWindow(g_hWnd, SW_SHOW);

    // Main loop
    MSG msg = { 0 };
    while (g_isRunning)
    {
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        Render();
    }

    CleanupDirectX();
    return 0;
}

/*
 * COMPILATION: cl.exe 06-SwapChainSetup.cpp /link d3d11.lib dxgi.lib user32.lib /SUBSYSTEM:WINDOWS
 *
 * SWAP CHAIN CONCEPTS:
 * - BufferCount: Number of back buffers (1 = double buffering)
 * - Present: Swap front and back buffers
 * - VSync: Synchronize with monitor refresh rate (prevents tearing)
 */
