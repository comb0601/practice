/*
 * DirectX 11 - Lesson 68: Index Buffers
 * Example 02: Indexed Quad Rendering
 *
 * Demonstrates using index buffers for efficient rendering.
 * Renders a quad (2 triangles) using only 4 vertices instead of 6.
 */

#include <windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

struct Vertex { float x, y, z; float r, g, b, a; };

IDXGISwapChain* g_swapChain = nullptr;
ID3D11Device* g_device = nullptr;
ID3D11DeviceContext* g_context = nullptr;
ID3D11RenderTargetView* g_rtv = nullptr;
ID3D11VertexShader* g_vs = nullptr;
ID3D11PixelShader* g_ps = nullptr;
ID3D11InputLayout* g_layout = nullptr;
ID3D11Buffer* g_vertexBuffer = nullptr;
ID3D11Buffer* g_indexBuffer = nullptr;

const char* shaderCode = R"(
struct VS_INPUT { float3 pos : POSITION; float4 col : COLOR; };
struct VS_OUTPUT { float4 pos : SV_POSITION; float4 col : COLOR; };
VS_OUTPUT VS(VS_INPUT input) {
    VS_OUTPUT output;
    output.pos = float4(input.pos, 1.0f);
    output.col = input.col;
    return output;
}
float4 PS(VS_OUTPUT input) : SV_TARGET { return input.col; }
)";

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (msg == WM_DESTROY) { PostQuitMessage(0); return 0; }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

bool InitDirectX(HWND hWnd)
{
    DXGI_SWAP_CHAIN_DESC scd = {};
    scd.BufferCount = 1;
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scd.OutputWindow = hWnd;
    scd.SampleDesc.Count = 1;
    scd.Windowed = TRUE;

    D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0,
        nullptr, 0, D3D11_SDK_VERSION, &scd, &g_swapChain, &g_device, nullptr, &g_context);

    ID3D11Texture2D* backBuffer;
    g_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
    g_device->CreateRenderTargetView(backBuffer, nullptr, &g_rtv);
    backBuffer->Release();
    g_context->OMSetRenderTargets(1, &g_rtv, nullptr);

    D3D11_VIEWPORT vp = { 0, 0, 800, 600, 0.0f, 1.0f };
    g_context->RSSetViewports(1, &vp);

    ID3DBlob *vsBlob, *psBlob;
    D3DCompile(shaderCode, strlen(shaderCode), nullptr, nullptr, nullptr, "VS", "vs_4_0", 0, 0, &vsBlob, nullptr);
    D3DCompile(shaderCode, strlen(shaderCode), nullptr, nullptr, nullptr, "PS", "ps_4_0", 0, 0, &psBlob, nullptr);

    g_device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, &g_vs);
    g_device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, &g_ps);

    D3D11_INPUT_ELEMENT_DESC layout[] = {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
    };
    g_device->CreateInputLayout(layout, 2, vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &g_layout);
    vsBlob->Release();
    psBlob->Release();

    // 4 vertices for a quad
    Vertex vertices[] = {
        {-0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f, 1.0f },  // Top-left (red)
        { 0.5f,  0.5f, 0.0f,  0.0f, 1.0f, 0.0f, 1.0f },  // Top-right (green)
        {-0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f, 1.0f },  // Bottom-left (blue)
        { 0.5f, -0.5f, 0.0f,  1.0f, 1.0f, 0.0f, 1.0f }   // Bottom-right (yellow)
    };

    D3D11_BUFFER_DESC bd = {};
    bd.ByteWidth = sizeof(vertices);
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    D3D11_SUBRESOURCE_DATA sd = { vertices };
    g_device->CreateBuffer(&bd, &sd, &g_vertexBuffer);

    // 6 indices for 2 triangles
    UINT indices[] = {
        0, 1, 2,  // First triangle (top-left, top-right, bottom-left)
        1, 3, 2   // Second triangle (top-right, bottom-right, bottom-left)
    };

    bd.ByteWidth = sizeof(indices);
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    sd.pSysMem = indices;
    g_device->CreateBuffer(&bd, &sd, &g_indexBuffer);

    return true;
}

void Render()
{
    float color[] = { 0.1f, 0.1f, 0.1f, 1.0f };
    g_context->ClearRenderTargetView(g_rtv, color);

    UINT stride = sizeof(Vertex), offset = 0;
    g_context->IASetVertexBuffers(0, 1, &g_vertexBuffer, &stride, &offset);
    g_context->IASetIndexBuffer(g_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
    g_context->IASetInputLayout(g_layout);
    g_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    g_context->VSSetShader(g_vs, nullptr, 0);
    g_context->PSSetShader(g_ps, nullptr, 0);

    g_context->DrawIndexed(6, 0, 0);  // Draw 6 indices

    g_swapChain->Present(1, 0);
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int)
{
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), 0, WndProc, 0, 0, hInst, 0, 0, 0, 0, L"DX11Quad", 0 };
    RegisterClassEx(&wc);
    HWND hWnd = CreateWindow(L"DX11Quad", L"Indexed Quad - 4 vertices, 6 indices",
        WS_OVERLAPPEDWINDOW, 100, 100, 800, 600, 0, 0, hInst, 0);

    InitDirectX(hWnd);
    ShowWindow(hWnd, SW_SHOW);

    MSG msg = {};
    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else Render();
    }

    if (g_indexBuffer) g_indexBuffer->Release();
    if (g_vertexBuffer) g_vertexBuffer->Release();
    if (g_layout) g_layout->Release();
    if (g_ps) g_ps->Release();
    if (g_vs) g_vs->Release();
    if (g_rtv) g_rtv->Release();
    if (g_context) g_context->Release();
    if (g_device) g_device->Release();
    if (g_swapChain) g_swapChain->Release();

    return 0;
}

/*
 * KEY CONCEPTS:
 * - 4 vertices instead of 6 (33% memory savings!)
 * - Indices reference vertices by index
 * - DrawIndexed() instead of Draw()
 * - Essential for complex meshes
 */
