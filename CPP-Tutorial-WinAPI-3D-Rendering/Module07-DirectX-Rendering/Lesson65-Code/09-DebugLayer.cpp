/*
 * DirectX 11 Tutorial - Lesson 65
 * Example 09: Debug Layer
 *
 * Demonstrates using the DirectX debug layer for development.
 * The debug layer provides detailed warnings and errors.
 */

#include <windows.h>
#include <d3d11.h>
#include <d3d11sdklayers.h>
#include <sstream>

#pragma comment(lib, "d3d11.lib")

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    // Create device with debug layer enabled
    UINT createDeviceFlags = D3D11_CREATE_DEVICE_DEBUG;

    ID3D11Device* device = nullptr;
    ID3D11DeviceContext* context = nullptr;

    HRESULT hr = D3D11CreateDevice(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        createDeviceFlags,  // Enable debug layer
        nullptr,
        0,
        D3D11_SDK_VERSION,
        &device,
        nullptr,
        &context
    );

    if (FAILED(hr))
    {
        MessageBox(nullptr,
                  L"Failed to create device with debug layer.\n\n"
                  L"Debug layer requires:\n"
                  L"- Windows SDK installed\n"
                  L"- Graphics Tools feature enabled\n\n"
                  L"Enable via: Settings → Apps → Optional Features → Graphics Tools",
                  L"Debug Layer Not Available",
                  MB_OK | MB_ICONWARNING);
        return 1;
    }

    // Get ID3D11Debug interface
    ID3D11Debug* debugInterface = nullptr;
    hr = device->QueryInterface(__uuidof(ID3D11Debug), (void**)&debugInterface);

    std::wstringstream ss;

    if (SUCCEEDED(hr))
    {
        ss << L"═══════════════════════════════════\n";
        ss << L"  DEBUG LAYER ENABLED\n";
        ss << L"═══════════════════════════════════\n\n";
        ss << L"The debug layer is active!\n\n";
        ss << L"All DirectX warnings and errors will be\n";
        ss << L"output to the Visual Studio Output window.\n\n";
        ss << L"To see debug output:\n";
        ss << L"1. Run this program from Visual Studio (F5)\n";
        ss << L"2. Check View → Output window\n";
        ss << L"3. Select \"Debug\" from the dropdown\n\n";

        // Trigger a warning by creating invalid buffer
        D3D11_BUFFER_DESC bufDesc = { 0 };
        bufDesc.ByteWidth = 0;  // Invalid - will trigger warning
        bufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

        ID3D11Buffer* buffer = nullptr;
        device->CreateBuffer(&bufDesc, nullptr, &buffer);
        // This should output a warning to debug console

        ss << L"An invalid buffer was created to trigger\n";
        ss << L"a debug warning. Check the Output window!\n";

        debugInterface->Release();
    }
    else
    {
        ss << L"Debug interface query failed.\n";
        ss << L"Debug layer may not be available.";
    }

    MessageBox(nullptr, ss.str().c_str(), L"Debug Layer", MB_OK | MB_ICONINFORMATION);

    // Cleanup
    context->Release();
    device->Release();

    return 0;
}

/*
 * COMPILATION: cl.exe /Zi 09-DebugLayer.cpp /link d3d11.lib user32.lib /SUBSYSTEM:WINDOWS /DEBUG
 *
 * ENABLING DEBUG LAYER:
 *
 * Windows 10/11:
 * 1. Settings → Apps → Apps & features
 * 2. Optional features → Add a feature
 * 3. Install "Graphics Tools"
 *
 * Or via PowerShell (admin):
 * Add-WindowsCapability -Online -Name "Tools.Graphics.DirectX~~~~0.0.1.0"
 *
 * DEBUG OUTPUT EXAMPLES:
 *
 * D3D11 WARNING: ID3D11Device::CreateBuffer: ByteWidth must be greater than 0
 * D3D11 ERROR: ID3D11DeviceContext::Draw: Vertex Buffer at slot 0 is not bound
 *
 * BENEFITS:
 * - Catches common mistakes
 * - Validates API usage
 * - Provides detailed error descriptions
 * - Essential for development
 *
 * PERFORMANCE:
 * - Debug layer adds significant overhead
 * - Only use in Debug builds
 * - Disable for Release builds
 */
