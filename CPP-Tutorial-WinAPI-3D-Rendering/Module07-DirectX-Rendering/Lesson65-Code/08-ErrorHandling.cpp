/*
 * DirectX 11 Tutorial - Lesson 65
 * Example 08: Error Handling
 *
 * Comprehensive error handling for DirectX applications.
 */

#include <windows.h>
#include <d3d11.h>
#include <comdef.h>
#include <sstream>

#pragma comment(lib, "d3d11.lib")

// Error handling macro
#define HR_CHECK(hr, msg) if (FAILED(hr)) { ShowError(hr, msg); return false; }

// Display error message
void ShowError(HRESULT hr, const wchar_t* operation)
{
    _com_error err(hr);
    std::wstringstream ss;
    ss << L"Operation: " << operation << L"\n\n";
    ss << L"HRESULT: 0x" << std::hex << hr << std::dec << L"\n";
    ss << L"Error: " << err.ErrorMessage();

    MessageBox(nullptr, ss.str().c_str(), L"DirectX Error",
              MB_OK | MB_ICONERROR);
}

// Safe device creation with error handling
bool CreateDeviceSafe()
{
    ID3D11Device* device = nullptr;
    ID3D11DeviceContext* context = nullptr;

    HRESULT hr = D3D11CreateDevice(
        nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0,
        nullptr, 0, D3D11_SDK_VERSION,
        &device, nullptr, &context
    );

    HR_CHECK(hr, L"D3D11CreateDevice");

    // Test buffer creation
    D3D11_BUFFER_DESC bufDesc = { 0 };
    bufDesc.Usage = D3D11_USAGE_DEFAULT;
    bufDesc.ByteWidth = 1024;
    bufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    ID3D11Buffer* buffer = nullptr;
    hr = device->CreateBuffer(&bufDesc, nullptr, &buffer);

    HR_CHECK(hr, L"CreateBuffer");

    // Cleanup
    if (buffer) buffer->Release();
    if (context) context->Release();
    if (device) device->Release();

    MessageBox(nullptr, L"All operations succeeded!", L"Success",
              MB_OK | MB_ICONINFORMATION);

    return true;
}

// Test various error conditions
void TestErrorConditions()
{
    ID3D11Device* device = nullptr;
    ID3D11DeviceContext* context = nullptr;

    // Create device
    D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0,
                     nullptr, 0, D3D11_SDK_VERSION,
                     &device, nullptr, &context);

    if (!device) return;

    // Test 1: Invalid buffer size (0 bytes)
    {
        D3D11_BUFFER_DESC bufDesc = { 0 };
        bufDesc.ByteWidth = 0;  // Invalid!
        bufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

        ID3D11Buffer* buffer = nullptr;
        HRESULT hr = device->CreateBuffer(&bufDesc, nullptr, &buffer);

        if (FAILED(hr))
        {
            ShowError(hr, L"CreateBuffer with 0 bytes (expected to fail)");
        }
    }

    // Test 2: Invalid usage combination
    {
        D3D11_BUFFER_DESC bufDesc = { 0 };
        bufDesc.ByteWidth = 1024;
        bufDesc.Usage = D3D11_USAGE_IMMUTABLE;  // Immutable...
        bufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;  // ...but writable? Invalid!
        bufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

        ID3D11Buffer* buffer = nullptr;
        HRESULT hr = device->CreateBuffer(&bufDesc, nullptr, &buffer);

        if (FAILED(hr))
        {
            ShowError(hr, L"Invalid usage flags (expected to fail)");
        }
    }

    // Cleanup
    context->Release();
    device->Release();
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    int choice = MessageBox(nullptr,
                           L"Choose test:\n\n"
                           L"YES = Safe creation (should succeed)\n"
                           L"NO = Error conditions (should fail)\n",
                           L"Error Handling Test",
                           MB_YESNOCANCEL | MB_ICONQUESTION);

    if (choice == IDYES)
    {
        CreateDeviceSafe();
    }
    else if (choice == IDNO)
    {
        TestErrorConditions();
    }

    return 0;
}

/*
 * COMPILATION: cl.exe 08-ErrorHandling.cpp /link d3d11.lib user32.lib /SUBSYSTEM:WINDOWS
 *
 * Common HRESULT values:
 * - S_OK (0x00000000): Success
 * - E_FAIL (0x80004005): Unspecified failure
 * - E_INVALIDARG (0x80070057): Invalid argument
 * - E_OUTOFMEMORY (0x8007000E): Out of memory
 * - DXGI_ERROR_INVALID_CALL: Invalid API call
 */
