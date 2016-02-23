#pragma once
/***********************************************
* Filename:  		D3DRenderer.h
* Date:      		2/21/2016
* Mod. Initials:	VG
* Author:    	    Volodymyr Golovan
* Purpose:   		Initialises the DirectX Pipeline
*                   Oversees the rendering order of
*                   all renderNodes
************************************************/

#pragma region DirectX Includes
#include <d3d11.h>
#include <DirectXMath.h>
#include <assert.h>
#include <atlbase.h>
#pragma comment(lib, "d3d11.lib")
using namespace DirectX;
#pragma endregion

#pragma region DirectxTK Includes
#include "DirectxTK\SpriteBatch.h"
#include "DirectxTK\SpriteFont.h"
#include "DirectxTK\CommonStates.h"
#include <memory>
#pragma endregion 
#include <map>
enum eFont { arial9,arial25};

class CRenderer
{
public:
	CRenderer();
	~CRenderer();

	void Init(HWND phWnd, int nWidth, int nHeight);
	void PreRenderInit();
	void Render();

	CComPtr<ID3D11Device> GetDevice() { return d3d_Device; }
	CComPtr<ID3D11DeviceContext> GetDeviceContext() { return d3d_ImmediateContext; }
	CComPtr<ID3D11RenderTargetView> GetRenderTargetView() { return d3d_renderTargetView; }
	CComPtr<IDXGISwapChain>         GetSwapChain() { return d3d_SwapChain; }


	SpriteBatch* Get2DRenderer() { return d3d_2dRenderer.get(); }
	SpriteFont* GetFontRenderer(eFont eFont) { return m_mFontRenderers[eFont].get(); }
	
private:
	HWND hWnd;
	int m_nWidth;
	int m_nHeight;


	//Pipeline 
	CComPtr<ID3D11Device>                                           d3d_Device;
	CComPtr<ID3D11DeviceContext>                          d3d_ImmediateContext;
	CComPtr<IDXGISwapChain>                                      d3d_SwapChain;
	CComPtr<ID3D11Texture2D>                                       d3d_ZBuffer;
	CComPtr<ID3D11DepthStencilView>                       d3d_depthStencilView;
	CComPtr<ID3D11SamplerState>                           d3d_WrapSamplerState;
	D3D11_VIEWPORT                                                d3d_viewport;
	CComPtr<ID3D11Texture2D>                                 d3d_BackbufferTex;
	CComPtr<ID3D11RenderTargetView>                       d3d_renderTargetView;
	CComPtr<ID3D11DepthStencilState>                     d3d_depthStencilState;


	//DirectXTK
	std::unique_ptr<SpriteBatch> d3d_2dRenderer;
	std::map<eFont, std::unique_ptr<SpriteFont>> m_mFontRenderers;
};

