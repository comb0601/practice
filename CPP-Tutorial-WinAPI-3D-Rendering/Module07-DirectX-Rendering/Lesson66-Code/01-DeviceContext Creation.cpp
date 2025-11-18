/*
 * DirectX 11 Tutorial - Lesson 66
 * Example 01: Device and Context Creation
 *
 * Demonstrates creating ID3D11Device and ID3D11DeviceContext
 * and querying their properties.
 */

#include <windows.h>
#include <d3d11.h>
#include <sstream>

#pragma comment(lib, "d3d11.lib")

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    ID3D11Device* device = nullptr;
    ID3D11DeviceContext* context = nullptr;
    D3D_FEATURE_LEVEL featureLevel;

    // Feature levels to try (in order of preference)
    D3D_FEATURE_LEVEL featureLevels[] = {
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0
    };

    UINT createFlags = 0;
    #ifdef _DEBUG
    createFlags |= D3D11_CREATE_DEVICE_DEBUG;
    #endif

    // Create device and context
    HRESULT hr = D3D11CreateDevice(
        nullptr,                    // Use default adapter
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        createFlags,
        featureLevels,
        ARRAYSIZE(featureLevels),
        D3D11_SDK_VERSION,
        &device,
        &featureLevel,
        &context
    );

    if (FAILED(hr))
    {
        MessageBox(nullptr, L"Failed to create device!", L"Error", MB_OK);
        return 1;
    }

    // Query device information
    std::wstringstream ss;
    ss << L"Device and Context Created!\n\n";

    // Feature level
    ss << L"Feature Level: ";
    switch (featureLevel)
    {
    case D3D_FEATURE_LEVEL_11_1: ss << L"11.1"; break;
    case D3D_FEATURE_LEVEL_11_0: ss << L"11.0"; break;
    case D3D_FEATURE_LEVEL_10_1: ss << L"10.1"; break;
    case D3D_FEATURE_LEVEL_10_0: ss << L"10.0"; break;
    }
    ss << L"\n\n";

    // Context type
    D3D11_DEVICE_CONTEXT_TYPE contextType = context->GetType();
    ss << L"Context Type: ";
    ss << (contextType == D3D11_DEVICE_CONTEXT_IMMEDIATE ? L"Immediate" : L"Deferred");
    ss << L"\n\n";

    // Check threading support
    D3D11_FEATURE_DATA_THREADING threading = {};
    device->CheckFeatureSupport(D3D11_FEATURE_THREADING, &threading, sizeof(threading));

    ss << L"Threading Support:\n";
    ss << L"  Concurrent Creates: " << (threading.DriverConcurrentCreates ? L"Yes" : L"No") << L"\n";
    ss << L"  Command Lists: " << (threading.DriverCommandLists ? L"Yes" : L"No") << L"\n";

    MessageBox(nullptr, ss.str().c_str(), L"Device Creation", MB_OK | MB_ICONINFORMATION);

    // Cleanup
    context->Release();
    device->Release();

    return 0;
}

/*
 * KEY CONCEPTS:
 *
 * Device: Factory for creating resources
 * - CreateBuffer(), CreateTexture2D(), CreateShader(), etc.
 *
 * Context: Interface for issuing rendering commands
 * - Draw(), SetShader(), UpdateSubresource(), etc.
 *
 * Context Types:
 * - Immediate: Commands execute immediately
 * - Deferred: Commands recorded to command list for later execution
 */
