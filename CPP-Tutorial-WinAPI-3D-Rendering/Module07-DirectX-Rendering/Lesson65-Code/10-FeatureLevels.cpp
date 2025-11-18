/*
 * DirectX 11 Tutorial - Lesson 65
 * Example 10: Feature Levels
 *
 * Demonstrates feature level detection and capability queries.
 */

#include <windows.h>
#include <d3d11.h>
#include <sstream>

#pragma comment(lib, "d3d11.lib")

const wchar_t* FeatureLevelName(D3D_FEATURE_LEVEL level)
{
    switch (level)
    {
    case D3D_FEATURE_LEVEL_11_1: return L"11.1";
    case D3D_FEATURE_LEVEL_11_0: return L"11.0";
    case D3D_FEATURE_LEVEL_10_1: return L"10.1";
    case D3D_FEATURE_LEVEL_10_0: return L"10.0";
    case D3D_FEATURE_LEVEL_9_3:  return L"9.3";
    case D3D_FEATURE_LEVEL_9_2:  return L"9.2";
    case D3D_FEATURE_LEVEL_9_1:  return L"9.1";
    default: return L"Unknown";
    }
}

std::wstring GetFeatureInfo(D3D_FEATURE_LEVEL level)
{
    std::wstringstream ss;

    ss << L"Feature Level " << FeatureLevelName(level) << L" supports:\n\n";

    switch (level)
    {
    case D3D_FEATURE_LEVEL_11_1:
    case D3D_FEATURE_LEVEL_11_0:
        ss << L"✓ Compute Shaders\n";
        ss << L"✓ Hull & Domain Shaders (Tessellation)\n";
        ss << L"✓ DirectCompute\n";
        ss << L"✓ Structured Buffers\n";
        ss << L"✓ Max Texture Size: 16384x16384\n";
        ss << L"✓ Shader Model: 5.0\n";
        break;

    case D3D_FEATURE_LEVEL_10_1:
    case D3D_FEATURE_LEVEL_10_0:
        ss << L"✓ Geometry Shaders\n";
        ss << L"✓ Stream Output\n";
        ss << L"✓ Max Texture Size: 8192x8192\n";
        ss << L"✓ Shader Model: 4.0\n";
        ss << L"✗ No Compute Shaders\n";
        ss << L"✗ No Tessellation\n";
        break;

    case D3D_FEATURE_LEVEL_9_3:
    case D3D_FEATURE_LEVEL_9_2:
    case D3D_FEATURE_LEVEL_9_1:
        ss << L"✓ Programmable Shaders\n";
        ss << L"✓ Max Texture Size: 4096x4096\n";
        ss << L"✓ Shader Model: 2.0/3.0\n";
        ss << L"✗ No Geometry Shaders\n";
        ss << L"✗ No Compute Shaders\n";
        ss << L"✗ No Tessellation\n";
        break;
    }

    return ss.str();
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    D3D_FEATURE_LEVEL requestedLevels[] = {
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
        D3D_FEATURE_LEVEL_9_3
    };

    ID3D11Device* device = nullptr;
    ID3D11DeviceContext* context = nullptr;
    D3D_FEATURE_LEVEL obtainedLevel;

    HRESULT hr = D3D11CreateDevice(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        0,
        requestedLevels,
        ARRAYSIZE(requestedLevels),
        D3D11_SDK_VERSION,
        &device,
        &obtainedLevel,
        &context
    );

    if (FAILED(hr))
    {
        MessageBox(nullptr, L"Failed to create device", L"Error", MB_OK);
        return 1;
    }

    // Build message
    std::wstringstream ss;
    ss << L"═══════════════════════════════════\n";
    ss << L"  FEATURE LEVEL DETECTION\n";
    ss << L"═══════════════════════════════════\n\n";
    ss << L"Obtained: Feature Level " << FeatureLevelName(obtainedLevel) << L"\n\n";
    ss << GetFeatureInfo(obtainedLevel);

    // Test all levels
    ss << L"\n───────────────────────────────────\n";
    ss << L"Available Feature Levels:\n";
    ss << L"───────────────────────────────────\n\n";

    for (int i = 0; i < ARRAYSIZE(requestedLevels); i++)
    {
        ID3D11Device* testDevice = nullptr;
        ID3D11DeviceContext* testContext = nullptr;
        D3D_FEATURE_LEVEL testLevel;

        HRESULT testHr = D3D11CreateDevice(
            nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0,
            &requestedLevels[i], 1, D3D11_SDK_VERSION,
            &testDevice, &testLevel, &testContext
        );

        if (SUCCEEDED(testHr))
        {
            ss << L"✓ " << FeatureLevelName(requestedLevels[i]);
            if (testLevel == obtainedLevel)
                ss << L" (Current)";
            ss << L"\n";
            testContext->Release();
            testDevice->Release();
        }
        else
        {
            ss << L"✗ " << FeatureLevelName(requestedLevels[i]) << L"\n";
        }
    }

    MessageBox(nullptr, ss.str().c_str(), L"Feature Levels",
              MB_OK | MB_ICONINFORMATION);

    context->Release();
    device->Release();

    return 0;
}

/*
 * COMPILATION: cl.exe 10-FeatureLevels.cpp /link d3d11.lib user32.lib /SUBSYSTEM:WINDOWS
 *
 * FEATURE LEVELS EXPLAINED:
 *
 * - Allow DirectX 11 API to run on older hardware
 * - Higher levels have more features
 * - Can check at runtime and adjust rendering accordingly
 *
 * PRACTICAL USAGE:
 *
 * if (featureLevel >= D3D_FEATURE_LEVEL_11_0)
 * {
 *     // Use compute shaders, tessellation
 * }
 * else if (featureLevel >= D3D_FEATURE_LEVEL_10_0)
 * {
 *     // Use geometry shaders
 * }
 * else
 * {
 *     // Basic rendering only
 * }
 */
