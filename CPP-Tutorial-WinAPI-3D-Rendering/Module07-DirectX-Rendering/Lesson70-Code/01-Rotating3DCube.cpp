/*
 * DirectX 11 - Lesson 70: Vertex Shaders & 3D Transformation
 * Complete 3D Rotating Cube Example
 *
 * Demonstrates:
 * - Full 3D transformation pipeline
 * - World, View, Projection matrices
 * - 3D cube with index buffer
 * - Continuous rotation animation
 */

#include <windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

using namespace DirectX;

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

struct Vertex { XMFLOAT3 pos; XMFLOAT4 color; };
struct MatrixBuffer { XMMATRIX world; XMMATRIX view; XMMATRIX projection; };

IDXGISwapChain* g_swapChain = nullptr;
ID3D11Device* g_device = nullptr;
ID3D11DeviceContext* g_context = nullptr;
ID3D11RenderTargetView* g_rtv = nullptr;
ID3D11DepthStencilView* g_dsv = nullptr;
ID3D11VertexShader* g_vs = nullptr;
ID3D11PixelShader* g_ps = nullptr;
ID3D11InputLayout* g_layout = nullptr;
ID3D11Buffer* g_vertexBuffer = nullptr;
ID3D11Buffer* g_indexBuffer = nullptr;
ID3D11Buffer* g_constantBuffer = nullptr;
ID3D11RasterizerState* g_rastState = nullptr;

float g_rotation = 0.0f;

const char* shaderCode = R"(
cbuffer MatrixBuffer : register(b0) {
    matrix world;
    matrix view;
    matrix projection;
};
struct VS_INPUT { float3 pos : POSITION; float4 color : COLOR; };
struct VS_OUTPUT { float4 pos : SV_POSITION; float4 color : COLOR; };
VS_OUTPUT VS(VS_INPUT input) {
    VS_OUTPUT output;
    float4 worldPos = mul(float4(input.pos, 1.0f), world);
    float4 viewPos = mul(worldPos, view);
    output.pos = mul(viewPos, projection);
    output.color = input.color;
    return output;
}
float4 PS(VS_OUTPUT input) : SV_TARGET { return input.color; }
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

    // Render target
    ID3D11Texture2D* backBuffer;
    g_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
    g_device->CreateRenderTargetView(backBuffer, nullptr, &g_rtv);
    backBuffer->Release();

    // Depth stencil
    D3D11_TEXTURE2D_DESC depthDesc = {};
    depthDesc.Width = 800;
    depthDesc.Height = 600;
    depthDesc.MipLevels = 1;
    depthDesc.ArraySize = 1;
    depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthDesc.SampleDesc.Count = 1;
    depthDesc.Usage = D3D11_USAGE_DEFAULT;
    depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

    ID3D11Texture2D* depthStencil;
    g_device->CreateTexture2D(&depthDesc, nullptr, &depthStencil);
    g_device->CreateDepthStencilView(depthStencil, nullptr, &g_dsv);
    depthStencil->Release();

    g_context->OMSetRenderTargets(1, &g_rtv, g_dsv);

    D3D11_VIEWPORT vp = { 0, 0, 800, 600, 0.0f, 1.0f };
    g_context->RSSetViewports(1, &vp);

    // Shaders
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

    // Cube vertices
    Vertex vertices[] = {
        // Front face
        {XMFLOAT3(-0.5f,  0.5f, -0.5f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)},
        {XMFLOAT3( 0.5f,  0.5f, -0.5f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)},
        {XMFLOAT3( 0.5f, -0.5f, -0.5f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)},
        {XMFLOAT3(-0.5f, -0.5f, -0.5f), XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f)},
        // Back face
        {XMFLOAT3(-0.5f,  0.5f,  0.5f), XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f)},
        {XMFLOAT3( 0.5f,  0.5f,  0.5f), XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f)},
        {XMFLOAT3( 0.5f, -0.5f,  0.5f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)},
        {XMFLOAT3(-0.5f, -0.5f,  0.5f), XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f)}
    };

    D3D11_BUFFER_DESC bd = {};
    bd.ByteWidth = sizeof(vertices);
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    D3D11_SUBRESOURCE_DATA sd = { vertices };
    g_device->CreateBuffer(&bd, &sd, &g_vertexBuffer);

    // Cube indices
    UINT indices[] = {
        0,1,2, 0,2,3,  // Front
        4,6,5, 4,7,6,  // Back
        4,5,1, 4,1,0,  // Top
        3,2,6, 3,6,7,  // Bottom
        4,0,3, 4,3,7,  // Left
        1,5,6, 1,6,2   // Right
    };

    bd.ByteWidth = sizeof(indices);
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    sd.pSysMem = indices;
    g_device->CreateBuffer(&bd, &sd, &g_indexBuffer);

    // Constant buffer
    bd.ByteWidth = sizeof(MatrixBuffer);
    bd.Usage = D3D11_USAGE_DYNAMIC;
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    g_device->CreateBuffer(&bd, nullptr, &g_constantBuffer);

    // Rasterizer state (disable backface culling to see all faces)
    D3D11_RASTERIZER_DESC rastDesc = {};
    rastDesc.FillMode = D3D11_FILL_SOLID;
    rastDesc.CullMode = D3D11_CULL_NONE;
    g_device->CreateRasterizerState(&rastDesc, &g_rastState);
    g_context->RSSetState(g_rastState);

    return true;
}

void Render()
{
    g_rotation += 0.01f;

    // Build matrices
    XMMATRIX world = XMMatrixRotationY(g_rotation) * XMMatrixRotationX(g_rotation * 0.5f);
    XMMATRIX view = XMMatrixLookAtLH(
        XMVectorSet(0.0f, 0.0f, -3.0f, 1.0f),
        XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f),
        XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
    );
    XMMATRIX projection = XMMatrixPerspectiveFovLH(XM_PIDIV4, 800.0f/600.0f, 0.1f, 100.0f);

    // Update constant buffer
    D3D11_MAPPED_SUBRESOURCE mapped;
    g_context->Map(g_constantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
    MatrixBuffer* data = (MatrixBuffer*)mapped.pData;
    data->world = XMMatrixTranspose(world);
    data->view = XMMatrixTranspose(view);
    data->projection = XMMatrixTranspose(projection);
    g_context->Unmap(g_constantBuffer, 0);

    // Clear
    float color[] = { 0.1f, 0.1f, 0.2f, 1.0f };
    g_context->ClearRenderTargetView(g_rtv, color);
    g_context->ClearDepthStencilView(g_dsv, D3D11_CLEAR_DEPTH, 1.0f, 0);

    // Render cube
    UINT stride = sizeof(Vertex), offset = 0;
    g_context->IASetVertexBuffers(0, 1, &g_vertexBuffer, &stride, &offset);
    g_context->IASetIndexBuffer(g_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
    g_context->IASetInputLayout(g_layout);
    g_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    g_context->VSSetShader(g_vs, nullptr, 0);
    g_context->VSSetConstantBuffers(0, 1, &g_constantBuffer);
    g_context->PSSetShader(g_ps, nullptr, 0);

    g_context->DrawIndexed(36, 0, 0);

    g_swapChain->Present(1, 0);
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int)
{
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), 0, WndProc, 0, 0, hInst, 0, 0, 0, 0, L"DX11Cube", 0 };
    RegisterClassEx(&wc);
    HWND hWnd = CreateWindow(L"DX11Cube", L"3D Rotating Cube - Lesson 70",
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

    // Cleanup
    if (g_rastState) g_rastState->Release();
    if (g_constantBuffer) g_constantBuffer->Release();
    if (g_indexBuffer) g_indexBuffer->Release();
    if (g_vertexBuffer) g_vertexBuffer->Release();
    if (g_layout) g_layout->Release();
    if (g_ps) g_ps->Release();
    if (g_vs) g_vs->Release();
    if (g_dsv) g_dsv->Release();
    if (g_rtv) g_rtv->Release();
    if (g_context) g_context->Release();
    if (g_device) g_device->Release();
    if (g_swapChain) g_swapChain->Release();

    return 0;
}

/*
 * This is your first complete 3D application!
 * You should see a colorful rotating cube.
 *
 * Key concepts demonstrated:
 * - 3D transformation pipeline (World, View, Projection)
 * - Depth testing with depth-stencil buffer
 * - Index buffer for efficient cube rendering
 * - Continuous animation with matrix updates
 * - Full 3D rendering setup
 */
