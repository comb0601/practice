/*
 * DirectX 11 - Lesson 69: HLSL Basics
 * Complete HLSL Shader Example with Vertex Shader
 *
 * Demonstrates:
 * - HLSL shader compilation
 * - Vertex shader transformation
 * - Constant buffer usage
 * - Matrix transformations
 */

#include <windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

using namespace DirectX;

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

struct Vertex { float x, y, z; float r, g, b, a; };

struct MatrixBuffer
{
    XMMATRIX world;
    XMMATRIX view;
    XMMATRIX projection;
};

IDXGISwapChain* g_swapChain = nullptr;
ID3D11Device* g_device = nullptr;
ID3D11DeviceContext* g_context = nullptr;
ID3D11RenderTargetView* g_rtv = nullptr;
ID3D11VertexShader* g_vs = nullptr;
ID3D11PixelShader* g_ps = nullptr;
ID3D11InputLayout* g_layout = nullptr;
ID3D11Buffer* g_vertexBuffer = nullptr;
ID3D11Buffer* g_constantBuffer = nullptr;

float g_rotation = 0.0f;

const char* shaderCode = R"(
cbuffer MatrixBuffer : register(b0)
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

struct VS_INPUT {
    float3 position : POSITION;
    float4 color : COLOR;
};

struct VS_OUTPUT {
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output;

    // Transform position through matrices
    float4 worldPos = mul(float4(input.position, 1.0f), worldMatrix);
    float4 viewPos = mul(worldPos, viewMatrix);
    output.position = mul(viewPos, projectionMatrix);

    // Pass through color
    output.color = input.color;

    return output;
}

float4 PS(VS_OUTPUT input) : SV_TARGET
{
    return input.color;
}
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

    // Compile shaders
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

    // Create triangle
    Vertex vertices[] = {
        { 0.0f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f, 1.0f },
        { 0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f, 1.0f },
        {-0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f, 1.0f }
    };

    D3D11_BUFFER_DESC bd = {};
    bd.ByteWidth = sizeof(vertices);
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    D3D11_SUBRESOURCE_DATA sd = { vertices };
    g_device->CreateBuffer(&bd, &sd, &g_vertexBuffer);

    // Create constant buffer
    bd.ByteWidth = sizeof(MatrixBuffer);
    bd.Usage = D3D11_USAGE_DYNAMIC;
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    g_device->CreateBuffer(&bd, nullptr, &g_constantBuffer);

    return true;
}

void Render()
{
    g_rotation += 0.01f;

    // Update matrices
    XMMATRIX world = XMMatrixRotationZ(g_rotation);
    XMMATRIX view = XMMatrixIdentity();
    XMMATRIX projection = XMMatrixPerspectiveFovLH(XM_PIDIV4, 800.0f/600.0f, 0.1f, 100.0f);
    view = XMMatrixTranslation(0.0f, 0.0f, 3.0f);

    // Update constant buffer
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    g_context->Map(g_constantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    MatrixBuffer* dataPtr = (MatrixBuffer*)mappedResource.pData;
    dataPtr->world = XMMatrixTranspose(world);
    dataPtr->view = XMMatrixTranspose(view);
    dataPtr->projection = XMMatrixTranspose(projection);
    g_context->Unmap(g_constantBuffer, 0);

    float color[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    g_context->ClearRenderTargetView(g_rtv, color);

    UINT stride = sizeof(Vertex), offset = 0;
    g_context->IASetVertexBuffers(0, 1, &g_vertexBuffer, &stride, &offset);
    g_context->IASetInputLayout(g_layout);
    g_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    g_context->VSSetShader(g_vs, nullptr, 0);
    g_context->VSSetConstantBuffers(0, 1, &g_constantBuffer);
    g_context->PSSetShader(g_ps, nullptr, 0);

    g_context->Draw(3, 0);
    g_swapChain->Present(1, 0);
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int)
{
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), 0, WndProc, 0, 0, hInst, 0, 0, 0, 0, L"DX11HLSL", 0 };
    RegisterClassEx(&wc);
    HWND hWnd = CreateWindow(L"DX11HLSL", L"HLSL Shaders - Rotating Triangle",
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

    if (g_constantBuffer) g_constantBuffer->Release();
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
 * This example demonstrates:
 * - HLSL constant buffers with matrices
 * - Vertex shader transformation pipeline
 * - Continuous animation with matrix updates
 * - Proper matrix transposition for HLSL
 */
