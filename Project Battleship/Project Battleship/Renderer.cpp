#include "Renderer.h"
#include "SeaTile.h"
#include "StateManager.h"
#include "InputManager.h"


CRenderer::CRenderer()
{
}


CRenderer::~CRenderer()
{	
}

void CRenderer::Init(HWND phWnd, int nWidth, int nHeight)
{
	//Geting information on our main window 
	hWnd = phWnd;
	m_nWidth = nWidth;
	m_nHeight = nHeight;

	UINT creationFlags = 0;
#ifdef _DEBUG
	// If the project is in a debug build, enable the debug layer.
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL featureLevels[6] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1
	};

	DXGI_SWAP_CHAIN_DESC swapDesc;
	ZeroMemory(&swapDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
	swapDesc.BufferCount = 1;
	swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	swapDesc.OutputWindow = hWnd;
	swapDesc.Windowed = true;
	swapDesc.BufferDesc.Width = nWidth;
	swapDesc.BufferDesc.Height = nHeight;
	swapDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapDesc.SampleDesc.Count = 1;
	swapDesc.SampleDesc.Quality = 0;


	D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, creationFlags, featureLevels, ARRAYSIZE(featureLevels), D3D11_SDK_VERSION
		, &swapDesc, &d3d_SwapChain, &d3d_Device, nullptr, &d3d_ImmediateContext);




	// Backbuffer //
	d3d_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&d3d_BackbufferTex));
	d3d_Device->CreateRenderTargetView(d3d_BackbufferTex, NULL, &d3d_renderTargetView);


	// Viewport //
	ZeroMemory(&d3d_viewport, sizeof(d3d_viewport));
	d3d_viewport.Width = (float)nWidth;
	d3d_viewport.Height = (float)nHeight;
	d3d_viewport.TopLeftX = 0;
	d3d_viewport.TopLeftY = 0;
	d3d_viewport.MinDepth = 0.0f;
	d3d_viewport.MaxDepth = 1.0f;

	//d3d_SwapChain->ResizeBuffers(swapDesc.BufferCount, m_nWidth, m_nHeight, swapDesc.BufferDesc.Format, swapDesc.Flags);
	//d3d_SwapChain->SetFullscreenState(fullscreeen, nullptr);


	//	// Z Buffer //
	D3D11_TEXTURE2D_DESC depth;
	ZeroMemory(&depth, sizeof(D3D11_TEXTURE2D_DESC));
	depth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depth.Format = DXGI_FORMAT_D32_FLOAT;
	depth.Width = swapDesc.BufferDesc.Width;
	depth.Height = swapDesc.BufferDesc.Height;
	depth.MipLevels = 1;
	depth.ArraySize = 1;
	depth.SampleDesc.Count = 1;
	depth.Usage = D3D11_USAGE_DEFAULT;
	d3d_Device->CreateTexture2D(&depth, nullptr, &d3d_ZBuffer);


	// Depth Stencil View // 
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	descDSV.Format = DXGI_FORMAT_D32_FLOAT;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	d3d_Device->CreateDepthStencilView(d3d_ZBuffer, &descDSV, &d3d_depthStencilView);



	// sampler state // 
	D3D11_SAMPLER_DESC samplerSample;
	ZeroMemory(&samplerSample, sizeof(D3D11_SAMPLER_DESC));
	samplerSample.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerSample.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerSample.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerSample.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	d3d_Device->CreateSamplerState(&samplerSample, &d3d_WrapSamplerState);


	D3D11_DEPTH_STENCIL_DESC dsDesc;
	ZeroMemory(&dsDesc, sizeof(dsDesc));
	// Depth test parameters
	dsDesc.DepthEnable = true;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;

	// Stencil test parameters
	dsDesc.StencilEnable = true;
	dsDesc.StencilReadMask = 0xFF;
	dsDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing
	dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing
	dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create depth stencil state
	d3d_Device->CreateDepthStencilState(&dsDesc, &d3d_depthStencilState);
	

	//Init DirectXTK
	d3d_2dRenderer.reset(new SpriteBatch(d3d_ImmediateContext.p));

	m_mFontRenderers[arial25].reset(new SpriteFont(d3d_Device, L"Assets/Arial25.font"));
	m_mFontRenderers[arial9].reset(new SpriteFont(d3d_Device, L"Assets/Arial9.font"));
}

void CRenderer::PreRenderInit()
{
	d3d_ImmediateContext->ClearRenderTargetView(d3d_renderTargetView, Colors::LightGray);
	d3d_ImmediateContext->ClearDepthStencilView(d3d_depthStencilView.p, D3D11_CLEAR_DEPTH, 1.0f, 0);
	d3d_ImmediateContext->RSSetViewports(1, &d3d_viewport);
	d3d_ImmediateContext->OMSetRenderTargets(1, &d3d_renderTargetView.p, d3d_depthStencilView);
	d3d_ImmediateContext->OMSetDepthStencilState(d3d_depthStencilState, 1);
	d3d_ImmediateContext->PSSetSamplers(0, 1, &d3d_WrapSamplerState.p);
}

void CRenderer::Render()
{
	PreRenderInit();
	





}