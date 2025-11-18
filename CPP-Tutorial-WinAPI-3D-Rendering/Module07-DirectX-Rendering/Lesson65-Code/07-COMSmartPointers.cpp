/*
 * DirectX 11 Tutorial - Lesson 65
 * Example 07: COM Smart Pointers
 *
 * Demonstrates using Microsoft::WRL::ComPtr for automatic COM object management.
 */

#include <windows.h>
#include <d3d11.h>
#include <wrl/client.h>  // For ComPtr

using Microsoft::WRL::ComPtr;

#pragma comment(lib, "d3d11.lib")

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    // Manual reference counting (old way)
    {
        ID3D11Device* device = nullptr;
        ID3D11DeviceContext* context = nullptr;

        D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0,
                         nullptr, 0, D3D11_SDK_VERSION,
                         &device, nullptr, &context);

        // Must manually release
        if (context) context->Release();
        if (device) device->Release();
    }

    // ComPtr automatic management (modern way)
    {
        ComPtr<ID3D11Device> device;
        ComPtr<ID3D11DeviceContext> context;

        HRESULT hr = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0,
                                      nullptr, 0, D3D11_SDK_VERSION,
                                      &device, nullptr, &context);

        if (SUCCEEDED(hr))
        {
            // Use device and context
            // Automatically released when ComPtr goes out of scope
            MessageBox(nullptr, L"Device created with ComPtr!\nAutomatically managed.",
                      L"Smart Pointers", MB_OK);
        }
    }  // ComPtr automatically calls Release()

    // ComPtr benefits
    {
        ComPtr<ID3D11Device> device;

        // GetAddressOf() for output parameters
        D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0,
                         nullptr, 0, D3D11_SDK_VERSION,
                         device.GetAddressOf(), nullptr, nullptr);

        // Get() for input parameters
        if (device.Get())
        {
            // Reset() to release and set to nullptr
            device.Reset();
        }

        // Detach() to get raw pointer and release ownership
        ID3D11Device* raw = device.Detach();
        if (raw) raw->Release();  // Must manually release

        // Attach() to take ownership of existing pointer
        ID3D11Device* existing = nullptr;
        D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0,
                         nullptr, 0, D3D11_SDK_VERSION,
                         &existing, nullptr, nullptr);
        device.Attach(existing);  // ComPtr now owns it
    }

    MessageBox(nullptr,
              L"ComPtr Benefits:\n\n"
              L"✓ Automatic Release()\n"
              L"✓ Exception safe\n"
              L"✓ Copy/move semantics\n"
              L"✓ Reference counting\n"
              L"✓ Less error-prone",
              L"Smart Pointers", MB_OK | MB_ICONINFORMATION);

    return 0;
}

/*
 * COMPILATION: cl.exe 07-COMSmartPointers.cpp /link d3d11.lib user32.lib /SUBSYSTEM:WINDOWS
 *
 * ComPtr Methods:
 * - Get(): Get raw pointer
 * - GetAddressOf(): Get address for output parameters
 * - ReleaseAndGetAddressOf(): Release current and get address
 * - Reset(): Release and set to nullptr
 * - Attach(): Take ownership of raw pointer
 * - Detach(): Release ownership, return raw pointer
 * - As(): QueryInterface to another type
 */
