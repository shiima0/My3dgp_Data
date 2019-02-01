#pragma once
#include"ResourceManager.h"
#include"framework.h"
#include <d3d11.h>
#include <d3dcompiler.h>
#include<DirectXMath.h>


class Shader
{
protected:
	ID3D11VertexShader*		VS; // 頂点シェーダ
	ID3D11PixelShader*		PS; // ピクセルシェーダ
	ID3D11InputLayout*		VertexLayout;

	HRESULT Compile(ID3D11Device*Device,int num,WCHAR* filename, ID3D11ShaderResourceView**SRView, D3D11_TEXTURE2D_DESC*TexDesc);

public:
	Shader() {};
	virtual ~Shader();

	

	bool Create(ID3D11Device*Device,int num,WCHAR* csofilename);
	void Activate(ID3D11DeviceContext * Context);
};