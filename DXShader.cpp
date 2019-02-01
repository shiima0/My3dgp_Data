#include "DXShader.h"


Shader::~Shader()
{
	if (VertexLayout) { VertexLayout->Release(); VertexLayout = NULL; }
	if (VS) { VS->Release(); VS = NULL; }
	if (PS) { PS->Release(); PS = NULL; }
}

//****************************************************************
//
//
//
//****************************************************************
//------------------------------------------------
//	�V�F�[�_�[�P�̃R���p�C��
//------------------------------------------------
HRESULT Shader::Compile(ID3D11Device*Device,int num,WCHAR* filename, ID3D11ShaderResourceView**SRView, D3D11_TEXTURE2D_DESC*TexDesc)
{
	DWORD ShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
	//ShaderFlags |= D3DCOMPILE_DEBUG;
	//ShaderFlags |= D3DCOMPILE_OPTIMIZATION_LEVEL3;

	
	// �R���p�C��
	HRESULT hr=S_OK;

	



	

	return hr;
}

//------------------------------------------------
//	�V�F�[�_�[�Z�b�g�R���p�C��
//------------------------------------------------
bool Shader::Create(ID3D11Device*Device,int num,WCHAR* filename)
{
	HRESULT hr = S_OK;

	ID3DBlob* VSBlob = NULL;
	// ���_�V�F�[�_
	//hr = Compile(Device,num,filename);
	if (FAILED(hr))
	{
		return false;
	}

	// ���_�V�F�[�_����
	hr = Device->CreateVertexShader(VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(), NULL, &VS);
	if (FAILED(hr))
	{
		VSBlob->Release();
		return false;
	}

	// ���̓��C�A�E�g
	D3D11_INPUT_ELEMENT_DESC layout3D[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 4*3, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 4*6, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 4*8, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = ARRAYSIZE(layout3D);

	// ���̓��C�A�E�g����
	hr = Device->CreateInputLayout(
		layout3D,
		numElements,
		VSBlob->GetBufferPointer(),
		VSBlob->GetBufferSize(),
		&VertexLayout
	);

	VSBlob->Release();
	if (FAILED(hr))
	{
		return false;
	}

	


	// �s�N�Z���V�F�[�_
	ID3DBlob* PSBlob = NULL;
	//hr = Compile(Device,num,filename);
	if (FAILED(hr))
	{
		return false;
	}

	// �s�N�Z���V�F�[�_����
	hr = Device->CreatePixelShader( PSBlob->GetBufferPointer(), PSBlob->GetBufferSize(), NULL, &PS );
	PSBlob->Release();
	if (FAILED(hr))
	{
		return false;
	}

	return true;
}

//****************************************************************
//
//
//
//****************************************************************
//------------------------------------------------
//	�L����
//------------------------------------------------
void Shader::Activate(ID3D11DeviceContext * Context)
{

	// ���̓��C�A�E�g�ݒ�
  Context->IASetInputLayout(VertexLayout);

  Context->VSSetShader(VS, NULL, 0);
  Context->PSSetShader(PS, NULL, 0);
	
  Context->GSSetShader(NULL, NULL, 0);
}

