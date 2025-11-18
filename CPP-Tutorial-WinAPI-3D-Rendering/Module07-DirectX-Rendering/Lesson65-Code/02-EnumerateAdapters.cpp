/*
 * DirectX 11 Tutorial - Lesson 65: DirectX Overview & Setup
 * Example 02: Enumerate Adapters
 *
 * This program enumerates all graphics adapters (GPUs) in the system
 * and displays detailed information about each one, including their outputs.
 *
 * Concepts demonstrated:
 * - DXGI (DirectX Graphics Infrastructure)
 * - Adapter enumeration
 * - Output (monitor) enumeration
 * - Display mode enumeration
 */

#include <windows.h>
#include <d3d11.h>
#include <dxgi1_2.h>
#include <sstream>
#include <vector>
#include <iomanip>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

// Structure to hold adapter information
struct AdapterInfo
{
    std::wstring description;
    UINT vendorId;
    UINT deviceId;
    SIZE_T dedicatedVideoMemory;
    SIZE_T dedicatedSystemMemory;
    SIZE_T sharedSystemMemory;
    std::vector<std::wstring> outputs;
    int displayModeCount;
};

// Function to get vendor name from vendor ID
std::wstring GetVendorName(UINT vendorId)
{
    switch (vendorId)
    {
    case 0x10DE: return L"NVIDIA";
    case 0x1002: return L"AMD";
    case 0x8086: return L"Intel";
    case 0x1414: return L"Microsoft (Software)";
    default: return L"Unknown";
    }
}

// Enumerate all display modes for an output
int EnumerateDisplayModes(IDXGIOutput* output)
{
    UINT numModes = 0;

    // Get the number of display modes
    output->GetDisplayModeList(
        DXGI_FORMAT_R8G8B8A8_UNORM,
        DXGI_ENUM_MODES_INTERLACED,
        &numModes,
        nullptr
    );

    return numModes;
}

// Enumerate all adapters
std::vector<AdapterInfo> EnumerateAdapters()
{
    std::vector<AdapterInfo> adapters;

    // Create DXGI factory
    IDXGIFactory1* factory = nullptr;
    HRESULT hr = CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)&factory);

    if (FAILED(hr))
    {
        return adapters;
    }

    // Enumerate adapters
    UINT adapterIndex = 0;
    IDXGIAdapter1* adapter = nullptr;

    while (factory->EnumAdapters1(adapterIndex, &adapter) != DXGI_ERROR_NOT_FOUND)
    {
        AdapterInfo info;
        DXGI_ADAPTER_DESC1 desc;

        if (SUCCEEDED(adapter->GetDesc1(&desc)))
        {
            info.description = desc.Description;
            info.vendorId = desc.VendorId;
            info.deviceId = desc.DeviceId;
            info.dedicatedVideoMemory = desc.DedicatedVideoMemory;
            info.dedicatedSystemMemory = desc.DedicatedSystemMemory;
            info.sharedSystemMemory = desc.SharedSystemMemory;

            // Enumerate outputs (monitors) for this adapter
            UINT outputIndex = 0;
            IDXGIOutput* output = nullptr;
            int totalDisplayModes = 0;

            while (adapter->EnumOutputs(outputIndex, &output) != DXGI_ERROR_NOT_FOUND)
            {
                DXGI_OUTPUT_DESC outputDesc;
                if (SUCCEEDED(output->GetDesc(&outputDesc)))
                {
                    std::wstringstream ss;
                    ss << L"Output " << outputIndex << L": " << outputDesc.DeviceName;

                    // Add resolution information
                    RECT rect = outputDesc.DesktopCoordinates;
                    ss << L" [" << (rect.right - rect.left) << L"x" << (rect.bottom - rect.top) << L"]";

                    // Add rotation
                    switch (outputDesc.Rotation)
                    {
                    case DXGI_MODE_ROTATION_IDENTITY: ss << L" (0°)"; break;
                    case DXGI_MODE_ROTATION_ROTATE90: ss << L" (90°)"; break;
                    case DXGI_MODE_ROTATION_ROTATE180: ss << L" (180°)"; break;
                    case DXGI_MODE_ROTATION_ROTATE270: ss << L" (270°)"; break;
                    }

                    info.outputs.push_back(ss.str());

                    // Count display modes
                    totalDisplayModes += EnumerateDisplayModes(output);
                }

                output->Release();
                outputIndex++;
            }

            info.displayModeCount = totalDisplayModes;
            adapters.push_back(info);
        }

        adapter->Release();
        adapterIndex++;
    }

    factory->Release();
    return adapters;
}

// Build message string with adapter information
std::wstring BuildAdapterMessage(const std::vector<AdapterInfo>& adapters)
{
    std::wstringstream ss;
    ss << L"═══════════════════════════════════════\n";
    ss << L"  DIRECTX GRAPHICS ADAPTERS DETECTED\n";
    ss << L"═══════════════════════════════════════\n\n";

    if (adapters.empty())
    {
        ss << L"No graphics adapters found!\n";
        return ss.str();
    }

    for (size_t i = 0; i < adapters.size(); i++)
    {
        const AdapterInfo& adapter = adapters[i];

        ss << L"┌─ ADAPTER " << (i + 1) << L" ───────────────────────┐\n";
        ss << L"│\n";
        ss << L"│ Description: " << adapter.description << L"\n";
        ss << L"│ Vendor: " << GetVendorName(adapter.vendorId)
           << L" (ID: 0x" << std::hex << adapter.vendorId << std::dec << L")\n";
        ss << L"│ Device ID: 0x" << std::hex << adapter.deviceId << std::dec << L"\n";
        ss << L"│\n";
        ss << L"│ ┌─ MEMORY ─────────────────────────┐\n";
        ss << L"│ │ Dedicated Video: "
           << (adapter.dedicatedVideoMemory / 1024 / 1024) << L" MB\n";
        ss << L"│ │ Dedicated System: "
           << (adapter.dedicatedSystemMemory / 1024 / 1024) << L" MB\n";
        ss << L"│ │ Shared System: "
           << (adapter.sharedSystemMemory / 1024 / 1024) << L" MB\n";
        ss << L"│ └──────────────────────────────────┘\n";
        ss << L"│\n";
        ss << L"│ ┌─ OUTPUTS ────────────────────────┐\n";

        if (adapter.outputs.empty())
        {
            ss << L"│ │ No active outputs detected\n";
        }
        else
        {
            for (const auto& output : adapter.outputs)
            {
                ss << L"│ │ " << output << L"\n";
            }
            ss << L"│ │ Total Display Modes: " << adapter.displayModeCount << L"\n";
        }

        ss << L"│ └──────────────────────────────────┘\n";
        ss << L"│\n";
        ss << L"└───────────────────────────────────┘\n\n";
    }

    ss << L"Total Adapters: " << adapters.size() << L"\n";

    return ss.str();
}

// Windows entry point
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

    // Enumerate all adapters
    std::vector<AdapterInfo> adapters = EnumerateAdapters();

    // Build message
    std::wstring message = BuildAdapterMessage(adapters);

    // Display in message box
    MessageBox(
        nullptr,
        message.c_str(),
        L"Graphics Adapter Enumeration",
        MB_OK | MB_ICONINFORMATION
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
 * 3. Set SubSystem to Windows
 * 4. Link: d3d11.lib dxgi.lib
 * 5. Build and Run
 *
 * Using command line:
 * cl.exe /EHsc 02-EnumerateAdapters.cpp /link d3d11.lib dxgi.lib user32.lib /SUBSYSTEM:WINDOWS
 *
 * EXPECTED OUTPUT:
 * ===============
 * A message box displaying information about all graphics adapters:
 * - Adapter name and description
 * - Vendor (NVIDIA, AMD, Intel, etc.)
 * - Memory allocation (dedicated video, dedicated system, shared)
 * - Connected outputs (monitors)
 * - Available display modes
 *
 * UNDERSTANDING THE OUTPUT:
 * ========================
 *
 * Dedicated Video Memory:
 * - Memory on the graphics card itself (fastest)
 * - Used for textures, vertex buffers, render targets
 *
 * Dedicated System Memory:
 * - System RAM reserved for GPU use
 * - Common on integrated GPUs
 *
 * Shared System Memory:
 * - System RAM that can be shared with GPU
 * - Slower than dedicated memory
 *
 * Vendor IDs:
 * - 0x10DE: NVIDIA
 * - 0x1002: AMD/ATI
 * - 0x8086: Intel
 * - 0x1414: Microsoft (WARP software renderer)
 *
 * LEARNING POINTS:
 * ===============
 * 1. DXGI (DirectX Graphics Infrastructure) manages low-level graphics tasks
 * 2. IDXGIFactory1 is used to enumerate adapters
 * 3. IDXGIAdapter1 represents a graphics adapter (GPU)
 * 4. IDXGIOutput represents a display output (monitor)
 * 5. Systems can have multiple adapters (e.g., integrated + discrete GPU)
 * 6. Each adapter can have multiple outputs (multi-monitor setups)
 *
 * COMMON SCENARIOS:
 * ================
 *
 * Laptop with Integrated + Discrete GPU:
 * - Adapter 0: Intel HD Graphics (integrated)
 * - Adapter 1: NVIDIA GeForce (discrete)
 *
 * Desktop with Single GPU:
 * - Adapter 0: NVIDIA/AMD GPU
 * - May show Microsoft Basic Display Adapter as fallback
 *
 * Multi-GPU Desktop:
 * - Multiple high-performance adapters
 * - Used for SLI/CrossFire or separate displays
 *
 * EXERCISES:
 * =========
 * 1. Modify to show detailed display mode information (resolution, refresh rate)
 * 2. Add filtering to show only hardware adapters (exclude software)
 * 3. Implement adapter selection for multi-GPU systems
 * 4. Add detection of GPU architecture (if available)
 * 5. Create a version that exports data to a text file
 * 6. Add HDR capability detection for each output
 * 7. Show which adapter is currently being used by the system
 *
 * PRACTICAL APPLICATIONS:
 * ======================
 * - GPU selection in games (choose between integrated/discrete)
 * - Multi-monitor support implementation
 * - Hardware requirements checking
 * - Performance profiling and optimization
 * - Debugging graphics issues
 */
