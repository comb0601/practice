/*
 * DirectX 11 Tutorial - Lesson 65: DirectX Overview & Setup
 * Example 03: Create Device
 *
 * This program demonstrates the basic steps to create a Direct3D 11 device
 * and device context, which are fundamental to all DirectX applications.
 *
 * Concepts demonstrated:
 * - D3D11CreateDevice function
 * - Device and DeviceContext interfaces
 * - Feature level selection
 * - Proper COM object cleanup
 */

#include <windows.h>
#include <d3d11.h>
#include <sstream>

#pragma comment(lib, "d3d11.lib")

// Function to create Direct3D device
bool CreateDirect3DDevice(
    ID3D11Device** outDevice,
    ID3D11DeviceContext** outContext,
    D3D_FEATURE_LEVEL* outFeatureLevel,
    std::wstring& errorMessage)
{
    // Define feature levels we want to support (in order of preference)
    D3D_FEATURE_LEVEL featureLevels[] = {
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0
    };

    UINT numFeatureLevels = ARRAYSIZE(featureLevels);

    // Device creation flags
    UINT createDeviceFlags = 0;

#ifdef _DEBUG
    // Enable debug layer in debug builds
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    // Create the Direct3D 11 device
    HRESULT hr = D3D11CreateDevice(
        nullptr,                    // Use default adapter
        D3D_DRIVER_TYPE_HARDWARE,   // Use hardware rendering
        nullptr,                    // No software module
        createDeviceFlags,          // Creation flags
        featureLevels,              // Array of feature levels
        numFeatureLevels,           // Number of feature levels
        D3D11_SDK_VERSION,          // SDK version
        outDevice,                  // Returns created device
        outFeatureLevel,            // Returns feature level
        outContext                  // Returns device context
    );

    if (FAILED(hr))
    {
        std::wstringstream ss;
        ss << L"Failed to create Direct3D device!\n\n";
        ss << L"Error Code: 0x" << std::hex << hr << L"\n\n";

        // Try to create with WARP (software) as fallback
        hr = D3D11CreateDevice(
            nullptr,
            D3D_DRIVER_TYPE_WARP,   // Use software rendering
            nullptr,
            createDeviceFlags,
            featureLevels,
            numFeatureLevels,
            D3D11_SDK_VERSION,
            outDevice,
            outFeatureLevel,
            outContext
        );

        if (SUCCEEDED(hr))
        {
            ss << L"Hardware device creation failed, but WARP (software) device created successfully.\n";
            ss << L"Note: Performance will be significantly reduced.\n";
            errorMessage = ss.str();
            return true;
        }
        else
        {
            ss << L"Both hardware and software device creation failed.\n";
            ss << L"Please update your graphics drivers.\n";
            errorMessage = ss.str();
            return false;
        }
    }

    return true;
}

// Query device capabilities
std::wstring QueryDeviceCapabilities(ID3D11Device* device, D3D_FEATURE_LEVEL featureLevel)
{
    std::wstringstream ss;

    // Feature level information
    ss << L"═══════════════════════════════════\n";
    ss << L"  DIRECT3D DEVICE INFORMATION\n";
    ss << L"═══════════════════════════════════\n\n";

    ss << L"Feature Level: ";
    switch (featureLevel)
    {
    case D3D_FEATURE_LEVEL_11_1: ss << L"11.1"; break;
    case D3D_FEATURE_LEVEL_11_0: ss << L"11.0"; break;
    case D3D_FEATURE_LEVEL_10_1: ss << L"10.1"; break;
    case D3D_FEATURE_LEVEL_10_0: ss << L"10.0"; break;
    default: ss << L"Unknown";
    }
    ss << L"\n\n";

    // Check threading support
    D3D11_FEATURE_DATA_THREADING threadingCaps = { 0 };
    device->CheckFeatureSupport(D3D11_FEATURE_THREADING, &threadingCaps, sizeof(threadingCaps));

    ss << L"┌─ THREADING SUPPORT ─────────────┐\n";
    ss << L"│ Concurrent Resources: " << (threadingCaps.DriverConcurrentCreates ? L"Yes" : L"No") << L"\n";
    ss << L"│ Command Lists: " << (threadingCaps.DriverCommandLists ? L"Yes" : L"No") << L"\n";
    ss << L"└─────────────────────────────────┘\n\n";

    // Check double precision shader support
    D3D11_FEATURE_DATA_DOUBLES doublesCaps = { 0 };
    device->CheckFeatureSupport(D3D11_FEATURE_DOUBLES, &doublesCaps, sizeof(doublesCaps));

    ss << L"┌─ SHADER CAPABILITIES ───────────┐\n";
    ss << L"│ Double Precision: " << (doublesCaps.DoublePrecisionFloatShaderOps ? L"Yes" : L"No") << L"\n";
    ss << L"└─────────────────────────────────┘\n\n";

    // Check format support for common formats
    ss << L"┌─ FORMAT SUPPORT ────────────────┐\n";

    DXGI_FORMAT formats[] = {
        DXGI_FORMAT_R8G8B8A8_UNORM,
        DXGI_FORMAT_R16G16B16A16_FLOAT,
        DXGI_FORMAT_R32G32B32A32_FLOAT,
        DXGI_FORMAT_D24_UNORM_S8_UINT
    };

    const wchar_t* formatNames[] = {
        L"R8G8B8A8_UNORM",
        L"R16G16B16A16_FLOAT",
        L"R32G32B32A32_FLOAT",
        L"D24_UNORM_S8_UINT"
    };

    for (int i = 0; i < 4; i++)
    {
        UINT support = 0;
        device->CheckFormatSupport(formats[i], &support);

        ss << L"│ " << formatNames[i] << L":\n";
        ss << L"│   Render Target: " << ((support & D3D11_FORMAT_SUPPORT_RENDER_TARGET) ? L"Yes" : L"No") << L"\n";
        ss << L"│   Texture2D: " << ((support & D3D11_FORMAT_SUPPORT_TEXTURE2D) ? L"Yes" : L"No") << L"\n";
    }

    ss << L"└─────────────────────────────────┘\n\n";

    // Get adapter information via DXGI
    IDXGIDevice* dxgiDevice = nullptr;
    if (SUCCEEDED(device->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice)))
    {
        IDXGIAdapter* adapter = nullptr;
        if (SUCCEEDED(dxgiDevice->GetAdapter(&adapter)))
        {
            DXGI_ADAPTER_DESC desc;
            if (SUCCEEDED(adapter->GetDesc(&desc)))
            {
                ss << L"┌─ ADAPTER INFORMATION ───────────┐\n";
                ss << L"│ " << desc.Description << L"\n";
                ss << L"│ Video Memory: " << (desc.DedicatedVideoMemory / 1024 / 1024) << L" MB\n";
                ss << L"└─────────────────────────────────┘\n";
            }
            adapter->Release();
        }
        dxgiDevice->Release();
    }

    return ss.str();
}

int WINAPI WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine,
    _In_ int nCmdShow)
{
    UNREFERENCED_PARAMETER(hInstance);
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    UNREFERENCED_PARAMETER(nCmdShow);

    ID3D11Device* device = nullptr;
    ID3D11DeviceContext* context = nullptr;
    D3D_FEATURE_LEVEL featureLevel;
    std::wstring message;

    // Create Direct3D device
    if (CreateDirect3DDevice(&device, &context, &featureLevel, message))
    {
        // Success - query capabilities
        message = QueryDeviceCapabilities(device, featureLevel);

        // Clean up
        context->Release();
        device->Release();

        MessageBox(nullptr, message.c_str(), L"Direct3D Device Created Successfully",
                   MB_OK | MB_ICONINFORMATION);
    }
    else
    {
        // Failure
        MessageBox(nullptr, message.c_str(), L"Direct3D Device Creation Failed",
                   MB_OK | MB_ICONERROR);
    }

    return 0;
}

/*
 * COMPILATION:
 * ===========
 * cl.exe /EHsc 03-CreateDevice.cpp /link d3d11.lib user32.lib /SUBSYSTEM:WINDOWS
 *
 * UNDERSTANDING THE CODE:
 * ======================
 *
 * D3D11CreateDevice Parameters:
 * -----------------------------
 * 1. pAdapter: Which GPU to use (nullptr = default)
 * 2. DriverType: HARDWARE, WARP (software), or REFERENCE
 * 3. Software: Handle to software rasterizer DLL (usually nullptr)
 * 4. Flags: Creation flags (D3D11_CREATE_DEVICE_DEBUG for debug layer)
 * 5. pFeatureLevels: Array of feature levels to try
 * 6. FeatureLevels: Number of feature levels in array
 * 7. SDKVersion: Always use D3D11_SDK_VERSION
 * 8. ppDevice: Receives created device
 * 9. pFeatureLevel: Receives selected feature level
 * 10. ppImmediateContext: Receives device context
 *
 * Device vs. Device Context:
 * -------------------------
 * - Device: Factory for creating resources (textures, buffers, etc.)
 * - Context: Used for rendering (draw calls, state changes, etc.)
 *
 * Think of it as:
 * - Device = Factory
 * - Context = Assembly line worker
 *
 * Feature Levels:
 * --------------
 * Allow DirectX 11 API to run on older hardware with reduced features
 * - 11_1: Full DirectX 11.1 (Windows 8+)
 * - 11_0: Full DirectX 11
 * - 10_1: DirectX 10.1 feature set
 * - 10_0: DirectX 10 feature set
 *
 * Driver Types:
 * ------------
 * - HARDWARE: Use GPU (fastest)
 * - WARP: Software rendering on CPU (slow but guaranteed to work)
 * - REFERENCE: Very slow, pixel-perfect reference implementation
 *
 * LEARNING POINTS:
 * ===============
 * 1. Always check HRESULT return values
 * 2. Release COM objects to avoid memory leaks
 * 3. Have a fallback to WARP for compatibility
 * 4. Enable debug layer during development
 * 5. Query device capabilities before using features
 *
 * COMMON ERRORS:
 * =============
 * E_INVALIDARG: Invalid parameter passed
 * E_FAIL: General failure (often driver issue)
 * DXGI_ERROR_UNSUPPORTED: Feature not supported
 *
 * DEBUG LAYER:
 * ===========
 * The D3D11_CREATE_DEVICE_DEBUG flag enables the debug layer which:
 * - Validates API usage
 * - Outputs warnings and errors to debugger
 * - Helps catch mistakes early
 * - Only available with Windows SDK installed
 *
 * EXERCISES:
 * =========
 * 1. Modify to explicitly choose a specific adapter
 * 2. Add comprehensive error handling for all failure cases
 * 3. Query additional device capabilities (compute shader support, etc.)
 * 4. Implement smart pointer wrapper (ComPtr) for automatic cleanup
 * 5. Add support for creating a deferred context
 * 6. Test what happens when debug layer is unavailable
 * 7. Measure device creation time
 */
