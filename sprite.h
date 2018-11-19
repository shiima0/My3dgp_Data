#pragma once
#include <d3d11.h>
#include<DirectXMath.h>



class sprite {

public:
	enum ALIGN
	{
		ALIGN_LEFTTOP,
		ALIGN_CENTERMIDDLE,
		ALIGN_CENTERBOTTOM,
	};

private:
	ID3D11VertexShader*     Vertex = NULL;
	ID3D11PixelShader*      PixelShader = NULL;
	ID3D11InputLayout*      Layout = NULL;
	ID3D11Buffer*           Buffer = NULL;
	ID3D11RasterizerState*  Raster = NULL;

	ID3D11ShaderResourceView* SRV = NULL;
	D3D11_TEXTURE2D_DESC     Tex2d_desc;
	ID3D11SamplerState*       Sample = NULL;

	ID3D11DepthStencilState*    Depth = NULL;
	//ID3D11BlendState*           BlendState = NULL;

public:
	sprite(ID3D11Device*,const wchar_t*);

	~sprite();

	void render(ID3D11DeviceContext * Context, float dx, float dy, float dw, float dh, float
		sx, float sy, float sw, float sh, float
		angle, float r, float g, float b, float a,
		ALIGN rotAlign= ALIGN_CENTERMIDDLE);
	



	struct vertex
	{
		DirectX::XMFLOAT3 Position;
		DirectX::XMFLOAT4 color;
		//UNIT4
		DirectX::XMFLOAT2 texcoord;
	};
};