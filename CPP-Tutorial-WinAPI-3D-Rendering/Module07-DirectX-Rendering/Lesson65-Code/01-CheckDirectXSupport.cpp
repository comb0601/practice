/*
 * DirectX 11 Tutorial - Lesson 65: DirectX Overview & Setup
 * Example 01: Check DirectX Support
 *
 * This program checks if DirectX 11 is supported on the current system
 * and displays information about available feature levels.
 *
 * Concepts demonstrated:
 * - Checking DirectX 11 availability
 * - Feature level detection
 * - Basic Windows programming
 * - Error handling
 */

#include <windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <sstream>
#include <string>

// Link necessary DirectX libraries
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

// Function to convert feature level to string
std::wstring FeatureLevelToString(D3D_FEATURE_LEVEL featureLevel)
{
    switch (featureLevel)
    {
    case D3D_FEATURE_LEVEL_11_1: return L"DirectX 11.1";
    case D3D_FEATURE_LEVEL_11_0: return L"DirectX 11.0";
    case D3D_FEATURE_LEVEL_10_1: return L"DirectX 10.1";
    case D3D_FEATURE_LEVEL_10_0: return L"DirectX 10.0";
    case D3D_FEATURE_LEVEL_9_3:  return L"DirectX 9.3";
    case D3D_FEATURE_LEVEL_9_2:  return L"DirectX 9.2";
    case D3D_FEATURE_LEVEL_9_1:  return L"DirectX 9.1";
    default: return L"Unknown";
    }
}

// Check DirectX 11 support
bool CheckDirectXSupport(std::wstring& message)
{
    // Array of feature levels to check (in order of preference)
    D3D_FEATURE_LEVEL featureLevels[] = {
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
        D3D_FEATURE_LEVEL_9_3,
        D3D_FEATURE_LEVEL_9_2,
        D3D_FEATURE_LEVEL_9_1
    };

    UINT numFeatureLevels = ARRAYSIZE(featureLevels);
    D3D_FEATURE_LEVEL featureLevel;

    // Attempt to create a Direct3D 11 device
    ID3D11Device* device = nullptr;
    ID3D11DeviceContext* context = nullptr;

    HRESULT hr = D3D11CreateDevice(
        nullptr,                    // Use default adapter
        D3D_DRIVER_TYPE_HARDWARE,   // Use hardware acceleration
        nullptr,                    // No software module
        0,                          // No flags
        featureLevels,              // Array of feature levels
        numFeatureLevels,           // Number of feature levels
        D3D11_SDK_VERSION,          // SDK version
        &device,                    // Returns device
        &featureLevel,              // Returns feature level
        &context                    // Returns device context
    );

    if (SUCCEEDED(hr))
    {
        // DirectX 11 is supported!
        std::wstringstream ss;
        ss << L"DirectX 11 is SUPPORTED on this system!\n\n";
        ss << L"Highest supported feature level: " << FeatureLevelToString(featureLevel) << L"\n\n";

        // Get adapter information
        IDXGIDevice* dxgiDevice = nullptr;
        if (SUCCEEDED(device->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice)))
        {
            IDXGIAdapter* adapter = nullptr;
            if (SUCCEEDED(dxgiDevice->GetAdapter(&adapter)))
            {
                DXGI_ADAPTER_DESC adapterDesc;
                if (SUCCEEDED(adapter->GetDesc(&adapterDesc)))
                {
                    ss << L"Graphics Adapter: " << adapterDesc.Description << L"\n";
                    ss << L"Dedicated Video Memory: " << (adapterDesc.DedicatedVideoMemory / 1024 / 1024) << L" MB\n";
                    ss << L"Dedicated System Memory: " << (adapterDesc.DedicatedSystemMemory / 1024 / 1024) << L" MB\n";
                    ss << L"Shared System Memory: " << (adapterDesc.SharedSystemMemory / 1024 / 1024) << L" MB\n";
                }
                adapter->Release();
            }
            dxgiDevice->Release();
        }

        // Check what other feature levels are supported
        ss << L"\nAll supported feature levels:\n";
        for (UINT i = 0; i < numFeatureLevels; i++)
        {
            D3D_FEATURE_LEVEL testLevel;
            ID3D11Device* testDevice = nullptr;
            ID3D11DeviceContext* testContext = nullptr;

            HRESULT testHr = D3D11CreateDevice(
                nullptr,
                D3D_DRIVER_TYPE_HARDWARE,
                nullptr,
                0,
                &featureLevels[i],
                1,
                D3D11_SDK_VERSION,
                &testDevice,
                &testLevel,
                &testContext
            );

            if (SUCCEEDED(testHr))
            {
                ss << L"  ✓ " << FeatureLevelToString(featureLevels[i]) << L"\n";
                testContext->Release();
                testDevice->Release();
            }
            else
            {
                ss << L"  ✗ " << FeatureLevelToString(featureLevels[i]) << L"\n";
            }
        }

        message = ss.str();

        // Clean up
        context->Release();
        device->Release();

        return true;
    }
    else
    {
        // DirectX 11 is not supported
        std::wstringstream ss;
        ss << L"DirectX 11 is NOT SUPPORTED on this system!\n\n";

        // Provide error information
        switch (hr)
        {
        case E_INVALIDARG:
            ss << L"Error: Invalid argument\n";
            break;
        case E_OUTOFMEMORY:
            ss << L"Error: Out of memory\n";
            break;
        case E_NOTIMPL:
            ss << L"Error: Not implemented\n";
            break;
        default:
            ss << L"Error code: 0x" << std::hex << hr << L"\n";
            break;
        }

        ss << L"\nYour system may not have DirectX 11 compatible hardware,\n";
        ss << L"or your graphics drivers may need to be updated.\n\n";
        ss << L"Please try:\n";
        ss << L"1. Updating your graphics drivers\n";
        ss << L"2. Installing Windows updates\n";
        ss << L"3. Checking if your hardware supports DirectX 11\n";

        message = ss.str();
        return false;
    }
}

// Windows entry point
int WINAPI WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine,
    _In_ int nCmdShow)
{
    // Unreferenced parameters
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    UNREFERENCED_PARAMETER(nCmdShow);

    // Check DirectX support
    std::wstring message;
    bool supported = CheckDirectXSupport(message);

    // Display result
    MessageBox(
        nullptr,
        message.c_str(),
        L"DirectX 11 Support Check",
        MB_OK | (supported ? MB_ICONINFORMATION : MB_ICONWARNING)
    );

    return 0;
}

/*
 * COMPILATION INSTRUCTIONS:
 * ========================
 *
 * Using Visual Studio:
 * 1. Create a new Empty C++ Project
 * 2. Add this file to the project
 * 3. Project Properties → Linker → System → SubSystem: Windows (/SUBSYSTEM:WINDOWS)
 * 4. Project Properties → Linker → Input → Additional Dependencies: d3d11.lib;dxgi.lib;d3dcompiler.lib
 * 5. Build and Run (F5)
 *
 * Using command line (with Visual Studio installed):
 * cl.exe /EHsc 01-CheckDirectXSupport.cpp /link d3d11.lib dxgi.lib d3dcompiler.lib user32.lib /SUBSYSTEM:WINDOWS
 *
 * EXPECTED OUTPUT:
 * ===============
 * A message box displaying:
 * - Whether DirectX 11 is supported
 * - The highest supported feature level
 * - Graphics adapter information
 * - List of all supported feature levels
 *
 * LEARNING POINTS:
 * ===============
 * 1. D3D11CreateDevice is used to create a Direct3D 11 device
 * 2. Feature levels allow DirectX 11 to run on older hardware
 * 3. The function returns S_OK (SUCCEEDED) if successful
 * 4. Device and context must be released to avoid memory leaks
 * 5. DXGI (DirectX Graphics Infrastructure) provides adapter information
 *
 * TROUBLESHOOTING:
 * ===============
 * If the program says DirectX 11 is not supported:
 * - Update your graphics drivers
 * - Ensure your GPU supports DirectX 11
 * - Check Windows Update for system updates
 *
 * EXERCISES:
 * =========
 * 1. Modify the program to test for DirectX 12 support
 * 2. Add a check for WARP (software) renderer as fallback
 * 3. Display the vendor ID and device ID from DXGI_ADAPTER_DESC
 * 4. Create a version that writes the information to a log file
 * 5. Add support for enumerating multiple adapters (for multi-GPU systems)
 */
