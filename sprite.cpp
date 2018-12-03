#include"framework.h"
#include"sprite.h"
#include"WICTextureLoader.h"
#include "ResourceManager.h"





bool ReadBinaryFile(const char*filename, BYTE**data, unsigned int &size) {
	//cso
	FILE*fp = 0;
	if (fopen_s(&fp, filename, "rb")) return false;
	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	*data = new  unsigned char[size];
	fread(*data, size, 1, fp);
	fclose(fp);

	return true;
}


sprite::sprite(ID3D11Device*Device,const wchar_t*file_name) {

	HRESULT hr;

	/*BYTE*data;
	unsigned int size;*/


	vertex vertices[] = {							//   R G B A
		{DirectX::XMFLOAT3(-0.5,+0.5,0),DirectX::XMFLOAT4(1,1,1,1),DirectX::XMFLOAT2(1,1)},
		{DirectX::XMFLOAT3(+0.5,+0.5,0),DirectX::XMFLOAT4(1,0,0,1),DirectX::XMFLOAT2(1,1)},
		{DirectX::XMFLOAT3(-0.5,-0.5,0),DirectX::XMFLOAT4(0,1,0,1),DirectX::XMFLOAT2(1,1)},
		{DirectX::XMFLOAT3(+0.5,-0.5,0),DirectX::XMFLOAT4(0,0,1,1),DirectX::XMFLOAT2(1,1)},
	};
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(vertices);		  //送りたい情報を換える
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;  //コンスタンとバッファーに換える
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = vertices;
	hr = Device->CreateBuffer(&bd, &InitData, &Buffer);
	if (FAILED(hr))
		return;

	//コンパイル済み頂点シェーダーオブジェクト読み込み
//	if (!ReadBinaryFile("sprite_vs.cso", &data, size))return;
//
//	//Create the Vertex Shader
////	hr = Device->CreateVertexShader(data, size, nullptr, &Vertex);
//
//	if (FAILED(hr)) {
//		return;
//	}

	// Define the input layout
	
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "COLOR"   , 0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "TEXCOORD", 0,DXGI_FORMAT_R32G32_FLOAT,0,28,D3D11_INPUT_PER_VERTEX_DATA,0}
	};
	UINT numElements = ARRAYSIZE(layout);
	

	ResourceManager::LoadVertexShader(Device, "sprite_vs.cso", layout, numElements, &Vertex, &Layout);
	
	
	ResourceManager::LoadPixelShader(Device, "sprite_ps.cso",&PixelShader);


	//ラスタライザーステート設定
	D3D11_RASTERIZER_DESC rsDesc;
	ZeroMemory(&rsDesc, sizeof(D3D11_RASTERIZER_DESC));
	rsDesc.FillMode = D3D11_FILL_SOLID;
	rsDesc.CullMode = D3D11_CULL_NONE;
	rsDesc.DepthClipEnable = FALSE;
	hr = Device->CreateRasterizerState(&rsDesc, &Raster);
	if (FAILED(hr))
		return;

	//テクスチャ読み込み

	ResourceManager::LoadShaderResourceView(Device, file_name,&SRV, &Tex2d_desc);
	



	//サンプラーステート設定
	
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	sampDesc.MipLODBias = 0;
	sampDesc.MaxAnisotropy = 16;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	hr = Device->CreateSamplerState(&sampDesc, &Sample);
	if (FAILED(hr))
		return;

	//一番最後に描画した画像を前に持ってきてナス！(詳細は要検索！)
	D3D11_DEPTH_STENCIL_DESC  depthDesc;
	ZeroMemory(&depthDesc, sizeof(depthDesc));
	depthDesc.DepthEnable= FALSE;
	depthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	depthDesc.DepthFunc = D3D11_COMPARISON_ALWAYS;
	depthDesc.StencilEnable = FALSE;
	hr = Device->CreateDepthStencilState(&depthDesc, &Depth);
	if (FAILED(hr))return;

	//画像を透過してるよ～ん(詳細は検索！検索！)
	/*D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(blendDesc));
	blendDesc.AlphaToCoverageEnable = FALSE;
	blendDesc.IndependentBlendEnable = FALSE;
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	
	hr = Device->CreateBlendState(&blendDesc, &BlendState);
	if (FAILED(hr))return;*/

}
sprite::~sprite() {

	//if (BlendState)BlendState->Release();
	if (Depth)Depth->Release();
	if (Sample)Sample->Release();
	ResourceManager::ReleaseShaderResourceView(SRV);
	if (Raster)Raster->Release();
	ResourceManager::ReleasePixelShader(PixelShader);
	ResourceManager::ReleaseVertexShader(Vertex,Layout);
	if (Buffer)Buffer->Release();
	
}
void sprite::render(ID3D11DeviceContext*Context, float dx, float dy,float dw,float dh,float sx,float sy,float sw,float sh,
					float angle,float r,float g, float b, float a,ALIGN rotAlign)
{
	
	
	//スクリーン空間左上大きさ１の■
	vertex vertices[] = {						   //R G B A
		{ DirectX::XMFLOAT3(0,0,0),DirectX::XMFLOAT4(r,g,b,a),DirectX::XMFLOAT2(sx,sy)},
		{ DirectX::XMFLOAT3(1,0,0),DirectX::XMFLOAT4(r,g,b,a),DirectX::XMFLOAT2(sx+sw,sy)},
		{ DirectX::XMFLOAT3(0,1,0),DirectX::XMFLOAT4(r,g,b,a),DirectX::XMFLOAT2(sx,sy+sh)},
		{ DirectX::XMFLOAT3(1,1,0),DirectX::XMFLOAT4(r,g,b,a),DirectX::XMFLOAT2(sx+sw,sy+sh)},
	};
	//アフィン変換
	
	//拡大・縮小
	for (int i = 0; i < 4; i++) {
		vertices[i].Position.x = dw*vertices[i].Position.x;
		vertices[i].Position.y = dh*vertices[i].Position.y;

	}
	//平行移動
	for (int i = 0; i < 4; i++) {
		vertices[i].Position.x = vertices[i].Position.x - dw / 2;
		vertices[i].Position.y = vertices[i].Position.y - dh / 2;
	}
	
	//回転
	for (int i = 0; i < 4; i++) {
		float  x = vertices[i].Position.x;
		float  y = vertices[i].Position.y;
		vertices[i].Position.x = x*cosf(angle*0.01745) - y*sinf(angle*0.01745);		//ラジアンに変える
		vertices[i].Position.y = x*sinf(angle*0.01745) + y*cosf(angle*0.01745);
	}
	//平行移動
	for (int i = 0; i < 4; i++) {
		vertices[i].Position.x = vertices[i].Position.x + dw / 2;
		vertices[i].Position.y = vertices[i].Position.y + dh / 2;
	}
	
	
	//平行移動
	for (int i = 0; i < 4; i++) {
		vertices[i].Position.x = vertices[i].Position.x + dx;
		vertices[i].Position.y = vertices[i].Position.y + dy;
	}
	
	//NDC変換
	vertex verticesNDC[4];
	for (int i = 0; i < 4; i++) 
	{
		verticesNDC[i] = vertices[i];

		verticesNDC[i].Position.x = vertices[i].Position.x / framework::SCREEN_WIDTH;
		verticesNDC[i].Position.x *= 2;
		verticesNDC[i].Position.x += -1;
		verticesNDC[i].Position.y = vertices[i].Position.y / framework::SCREEN_HEIGHT;
		verticesNDC[i].Position.y *=-2;
		verticesNDC[i].Position.y +=+1;

		verticesNDC[i].texcoord.x=(vertices[i].texcoord.x/Tex2d_desc.Width);
		verticesNDC[i].texcoord.y = (vertices[i].texcoord.y / Tex2d_desc.Height);


	}

		D3D11_MAPPED_SUBRESOURCE  mapped;
		Context->Map(Buffer, 0, D3D11_MAP_WRITE_DISCARD,0, &mapped);
		memcpy(mapped.pData,verticesNDC,sizeof(verticesNDC));
		Context->Unmap(Buffer, 0);
	
	
		// Set vertex buffer
		UINT stride = sizeof(vertex);
		UINT offset = 0;
		Context->IASetVertexBuffers(0, 1, &Buffer, &stride, &offset);

		// Set primitive topology
		Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

		// Set the input layout
		Context->IASetInputLayout(Layout);

		//State
		Context->RSSetState(Raster);

		
		
		// Render a triangle
		Context->VSSetShader(Vertex, NULL, 0);
		Context->PSSetShader(PixelShader,NULL, 0);
		Context->PSSetShaderResources(0, 1, &SRV);
		Context->PSSetSamplers(0, 1, &Sample);

		//depth
		Context->OMSetDepthStencilState(Depth, 0);

		//Blendstate
		//Context->OMSetBlendState(BlendState, nullptr, 0xffffffff);


		Context->Draw(4, 0);
		

}


	


