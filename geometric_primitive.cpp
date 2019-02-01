#include"geometrice_primitive.h"
#include"ResourceManager.h"
#include"framework.h"

geometric_primitive::geometric_primitive(ID3D11Device*Device) {

	HRESULT hr;


	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "NORMAL"  , 0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 },
	};
	UINT numElements = ARRAYSIZE(layout);

	ResourceManager::LoadVertexShader(Device, "geometric_primitive_vs.cso", layout, numElements, &Vertex, &Layout);
	ResourceManager::LoadPixelShader(Device, "geometric_primitive_ps.cso", &Pixel);


	D3D11_RASTERIZER_DESC rsDesc;
	ZeroMemory(&rsDesc, sizeof(D3D11_RASTERIZER_DESC));
	//WIREFRAME
	rsDesc.FillMode = D3D11_FILL_WIREFRAME;
	rsDesc.CullMode = D3D11_CULL_BACK;
	rsDesc.FrontCounterClockwise = FALSE;
	rsDesc.DepthClipEnable = TRUE;
	hr = Device->CreateRasterizerState(&rsDesc, &RSWireframe);
	if (FAILED(hr))return;

	//SOLID
	ZeroMemory(&rsDesc, sizeof(D3D11_RASTERIZER_DESC));
	rsDesc.FillMode = D3D11_FILL_SOLID;
	rsDesc.CullMode = D3D11_CULL_BACK;
	rsDesc.FrontCounterClockwise = FALSE;
	rsDesc.DepthClipEnable = TRUE;
	hr = Device->CreateRasterizerState(&rsDesc, &RSsolid);
	if (FAILED(hr))return;

	//深度ステンシルステート
	
	D3D11_DEPTH_STENCIL_DESC  depthDesc;
	ZeroMemory(&depthDesc, sizeof(depthDesc));
	depthDesc.DepthEnable = TRUE;
	depthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthDesc.StencilEnable = FALSE;
	depthDesc.StencilReadMask = 0xff;
	depthDesc.StencilWriteMask = 0xff;
	hr = Device->CreateDepthStencilState(&depthDesc, &Depth);
	if (FAILED(hr))return;

}

void geometric_primitive::BufferEdit(ID3D11Device*Device,
										 vertex*vertices,
										 int numVertices,
										 u_int* indeices,
										 int numIndex) {
	HRESULT hr;
//	UINT  numindeices;

	D3D11_BUFFER_DESC bd;
	D3D11_SUBRESOURCE_DATA sub_data;
	//頂点バッファ
	ZeroMemory(&bd, sizeof(bd));
	ZeroMemory(&sub_data, sizeof(sub_data));
	//bd.ByteWidth = sizeof(vertices);
	bd.ByteWidth = numVertices*sizeof(vertex);
	bd.Usage = D3D11_USAGE_IMMUTABLE;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;

	sub_data.pSysMem = vertices;
	sub_data.SysMemPitch = 0;
	sub_data.SysMemSlicePitch = 0;
	hr = Device->CreateBuffer(&bd, &sub_data, &Buffer);
	if (FAILED(hr))return;
	//インデックスバッファ
	ZeroMemory(&bd, sizeof(bd));
	ZeroMemory(&sub_data, sizeof(sub_data));
	//bd.ByteWidth = sizeof(indeices);
	bd.ByteWidth = numIndex*sizeof(u_int);
	bd.Usage = D3D11_USAGE_IMMUTABLE;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;

	sub_data.pSysMem = indeices;
	sub_data.SysMemPitch = 0;
	sub_data.SysMemSlicePitch = 0;

	// numindeices = numIndex;
	hr = Device->CreateBuffer(&bd, &sub_data, &IBuffer);
	if (FAILED(hr))return;

	//コンスタントバッファ
	ZeroMemory(&bd, sizeof(bd));
	bd.ByteWidth = sizeof(cbuffer);
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;


	hr = Device->CreateBuffer(&bd, nullptr, &CBuffer);
	if (FAILED(hr))return;
}
geometric_primitive::~geometric_primitive() 
{
	if (CBuffer)CBuffer->Release();
	if (IBuffer)IBuffer->Release();
	if (Buffer) Buffer->Release();
	if (Depth)Depth->Release();
	if (RSsolid)RSsolid->Release();
	if (RSWireframe)RSWireframe->Release();
	if (Layout)Layout->Release();

	

	ResourceManager::ReleasePixelShader(Pixel);
	ResourceManager::ReleaseVertexShader(Vertex, Layout);
	


}
void geometric_primitive::render(ID3D11DeviceContext*Context,
	const DirectX::XMFLOAT4X4&world_view,
	const DirectX::XMFLOAT4X4& worldM,
	const DirectX::XMFLOAT4& light,
	const DirectX::XMFLOAT4& Material_color, bool  bWareframe)
{
	//頂点バッファ||インデックスバッファが無ければ終了
	if (!Buffer || !IBuffer)return;


	cbuffer cb;
	cb.world_view_projection = world_view;
	cb.world	 =				   worldM;
	cb.material_color =		   Material_color;
	cb.light_direction =	   light;
	Context->UpdateSubresource(CBuffer, 0, NULL, &cb, 0, 0);	//定数バッファにコピー
	Context->VSSetConstantBuffers(0, 1, &CBuffer);		   //シェーダーにデーターを渡す

	// Set vertex buffer
	UINT stride = sizeof(vertex);
	UINT offset = 0;
	Context->IASetVertexBuffers(0, 1, &Buffer, &stride, &offset);

	Context->IASetIndexBuffer(IBuffer, DXGI_FORMAT_R32_UINT, 0);



	// Set primitive topology
	Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Set the input layout
	Context->IASetInputLayout(Layout);

	//State
	
	if(bWareframe) Context->RSSetState(RSWireframe);
	else          Context->RSSetState(RSsolid);

	// Render a triangle
	Context->VSSetShader(Vertex, NULL, 0);
	Context->PSSetShader(Pixel, NULL, 0);
	//depth
	Context->OMSetDepthStencilState(Depth, 0);
	//index付き描画
	D3D11_BUFFER_DESC buffer_desc;
	IBuffer->GetDesc(&buffer_desc);
	Context->DrawIndexed(buffer_desc.ByteWidth / sizeof(u_int), 0, 0);
	//こっちでも可
	//Context->DrawIndexed(numindeices,0,0);
}

geometric_Cube::geometric_Cube(ID3D11Device*Device):
				geometric_primitive(Device)
{

	vertex vertices[24] = {};
	u_int  indeices[36] = {};
	int face=0;
	bool YABAIDESYO = FALSE;

	//四角形の図
	// top-side 視点Y
	//TOP
	//01
	//23
	face = 0;
	vertices[face * 4 + 0].Position = DirectX::XMFLOAT3(-0.5f, +0.5f, +0.5f);
	vertices[face * 4 + 1].Position = DirectX::XMFLOAT3(+0.5f, +0.5f, +0.5f);
	vertices[face * 4 + 2].Position = DirectX::XMFLOAT3(-0.5f, +0.5f, -0.5f);
	vertices[face * 4 + 3].Position = DirectX::XMFLOAT3(+0.5f, +0.5f, -0.5f);
	
	vertices[face * 4 + 0].normal = DirectX::XMFLOAT3(+0.0f, +1.0f, +0.0f);
	vertices[face * 4 + 1].normal = DirectX::XMFLOAT3(+0.0f, +1.0f, +0.0f);
	vertices[face * 4 + 2].normal = DirectX::XMFLOAT3(+0.0f, +1.0f, +0.0f);
	vertices[face * 4 + 3].normal = DirectX::XMFLOAT3(+0.0f, +1.0f, +0.0f);

	indeices[face * 6 + 0] = face * 4 + 0;		//DirectXの定義で表面は時計回りに作らないといけない
	indeices[face * 6 + 1] = face * 4 + 1;
	indeices[face * 6 + 2] = face * 4 + 2;
	indeices[face * 6 + 3] = face * 4 + 1;
	indeices[face * 6 + 4] = face * 4 + 3;
	indeices[face * 6 + 5] = face * 4 + 2;
		


	//bottom-side
	//0--------1
	//|		   |
	//|		   |
	//|		   |
	//2--------3
	face += 1;
	vertices[face * 4 + 0].Position = DirectX::XMFLOAT3(-0.5f, -0.5f, +0.5f);
	vertices[face * 4 + 1].Position = DirectX::XMFLOAT3(+0.5f, -0.5f, +0.5f);
	vertices[face * 4 + 2].Position = DirectX::XMFLOAT3(-0.5f, -0.5f, -0.5f);
	vertices[face * 4 + 3].Position = DirectX::XMFLOAT3(+0.5f, -0.5f, -0.5f);

	vertices[face * 4 + 0].normal = DirectX::XMFLOAT3(+0.0f, -1.0f, +0.0f);
	vertices[face * 4 + 1].normal = DirectX::XMFLOAT3(+0.0f, -1.0f, +0.0f);
	vertices[face * 4 + 2].normal = DirectX::XMFLOAT3(+0.0f, -1.0f, +0.0f);
	vertices[face * 4 + 3].normal = DirectX::XMFLOAT3(+0.0f, -1.0f, +0.0f);

	indeices[face * 6 + 0] = face * 4 + 0;
	indeices[face * 6 + 1] = face * 4 + 2;
	indeices[face * 6 + 2] = face * 4 + 1;
	indeices[face * 6 + 3] = face * 4 + 1;
	indeices[face * 6 + 4] = face * 4 + 2;
	indeices[face * 6 + 5] = face * 4 + 3;

	//front-side
	//0--------1
	//|		   |
	//|		   |
	//|		   |
	//2--------3
	face += 1;
	vertices[face * 4 + 0].Position = DirectX::XMFLOAT3(-0.5f, +0.5f, -0.5f);
	vertices[face * 4 + 1].Position = DirectX::XMFLOAT3(+0.5f, +0.5f, -0.5f);
	vertices[face * 4 + 2].Position = DirectX::XMFLOAT3(-0.5f, -0.5f, -0.5f);
	vertices[face * 4 + 3].Position = DirectX::XMFLOAT3(+0.5f, -0.5f, -0.5f);

	vertices[face * 4 + 0].normal = DirectX::XMFLOAT3(+0.0f, +0.0f, -1.0f);
	vertices[face * 4 + 1].normal = DirectX::XMFLOAT3(+0.0f, +0.0f, -1.0f);
	vertices[face * 4 + 2].normal = DirectX::XMFLOAT3(+0.0f, +0.0f, -1.0f);
	vertices[face * 4 + 3].normal = DirectX::XMFLOAT3(+0.0f, +0.0f, -1.0f);

	indeices[face * 6 + 0] = face * 4 + 0;
	indeices[face * 6 + 1] = face * 4 + 1;
	indeices[face * 6 + 2] = face * 4 + 2;
	indeices[face * 6 + 3] = face * 4 + 1;
	indeices[face * 6 + 4] = face * 4 + 3;
	indeices[face * 6 + 5] = face * 4 + 2;

	//back-side				//半時計回り
	//0--------1
	//|		   |
	//|		   |
	//|		   |
	//2--------3
	face += 1;
	
	vertices[face * 4 + 0].Position = DirectX::XMFLOAT3(-0.5f, +0.5f, +0.5f);
	vertices[face * 4 + 1].Position = DirectX::XMFLOAT3(+0.5f, +0.5f, +0.5f);
	vertices[face * 4 + 2].Position = DirectX::XMFLOAT3(-0.5f, -0.5f, +0.5f);
	vertices[face * 4 + 3].Position = DirectX::XMFLOAT3(+0.5f, -0.5f, +0.5f);

	vertices[face * 4 + 0].normal = DirectX::XMFLOAT3(+0.0f, +0.0f, +1.0f);
	vertices[face * 4 + 1].normal = DirectX::XMFLOAT3(+0.0f, +0.0f, +1.0f);
	vertices[face * 4 + 2].normal = DirectX::XMFLOAT3(+0.0f, +0.0f, +1.0f);
	vertices[face * 4 + 3].normal = DirectX::XMFLOAT3(+0.0f, +0.0f, +1.0f);

	indeices[face * 6 + 0] = face * 4 + 0;
	indeices[face * 6 + 1] = face * 4 + 2;
	indeices[face * 6 + 2] = face * 4 + 1;
	indeices[face * 6 + 3] = face * 4 + 1;
	indeices[face * 6 + 4] = face * 4 + 2;
	indeices[face * 6 + 5] = face * 4 + 3;

	//right-side
	//0--------1
	//|		   |
	//|		   |
	//|		   |
	//2--------3
	face += 1;
	
	vertices[face * 4 + 0].Position = DirectX::XMFLOAT3(+0.5f, +0.5f, -0.5f);
	vertices[face * 4 + 1].Position = DirectX::XMFLOAT3(+0.5f, +0.5f, +0.5f);
	vertices[face * 4 + 2].Position = DirectX::XMFLOAT3(+0.5f, -0.5f, -0.5f);
	vertices[face * 4 + 3].Position = DirectX::XMFLOAT3(+0.5f, -0.5f, +0.5f);

	vertices[face * 4 + 0].normal = DirectX::XMFLOAT3(+1.0f, +0.0f, +0.0f);
	vertices[face * 4 + 1].normal = DirectX::XMFLOAT3(+1.0f, +0.0f, +0.0f);
	vertices[face * 4 + 2].normal = DirectX::XMFLOAT3(+1.0f, +0.0f, +0.0f);
	vertices[face * 4 + 3].normal = DirectX::XMFLOAT3(+1.0f, +0.0f, +0.0f);

	indeices[face * 6 + 0] = face * 4 + 0;
	indeices[face * 6 + 1] = face * 4 + 1;
	indeices[face * 6 + 2] = face * 4 + 2;
	indeices[face * 6 + 3] = face * 4 + 1;
	indeices[face * 6 + 4] = face * 4 + 3;
	indeices[face * 6 + 5] = face * 4 + 2;

	//left-side
	//0--------1
	//|		   |
	//|		   |
	//|		   |
	//2--------3
	face += 1;
	
	vertices[face * 4 + 0].Position = DirectX::XMFLOAT3(-0.5f, +0.5f, -0.5f);
	vertices[face * 4 + 1].Position = DirectX::XMFLOAT3(-0.5f, +0.5f, +0.5f);
	vertices[face * 4 + 2].Position = DirectX::XMFLOAT3(-0.5f, -0.5f, -0.5f);
	vertices[face * 4 + 3].Position = DirectX::XMFLOAT3(-0.5f, -0.5f, +0.5f);

	vertices[face * 4 + 0].normal = DirectX::XMFLOAT3(-1.0f, +0.0f, +0.0f);
	vertices[face * 4 + 1].normal = DirectX::XMFLOAT3(-1.0f, +0.0f, +0.0f);
	vertices[face * 4 + 2].normal = DirectX::XMFLOAT3(-1.0f, +0.0f, +0.0f);
	vertices[face * 4 + 3].normal = DirectX::XMFLOAT3(-1.0f, +0.0f, +0.0f);

	indeices[face * 6 + 0] = face * 4 + 0;
	indeices[face * 6 + 1] = face * 4 + 2;
	indeices[face * 6 + 2] = face * 4 + 1;
	indeices[face * 6 + 3] = face * 4 + 1;
	indeices[face * 6 + 4] = face * 4 + 2;
	indeices[face * 6 + 5] = face * 4 + 3;


	BufferEdit(Device, vertices,24, indeices, 36);
	

}
geometric_Cylinder::geometric_Cylinder(ID3D11Device*Device,int num):
	geometric_primitive(Device), NUM(num) {

	vertex vertices[24+2] = {};
	u_int  indices[24*3] = {};

	int ofsV = 0, ofsI = 0;
	
	const float DANGLE = DirectX::XMConvertToRadians(360.0f / 6);

	//円柱の図
	//TOP
	vertices[ofsV+0].Position = DirectX::XMFLOAT3(0, 0.5f, 0);   //中心の点

	for (int n = 0; n < 6; n++) 
	{
		vertices[ofsV+1+n].Position = DirectX::XMFLOAT3(cos(DANGLE * n), 0.5f, sin(DANGLE * n));
	}
	
	for (int n = 0; n < 7; n++) {
		vertices[ofsV+n].normal = DirectX::XMFLOAT3(0, 0.5f, 0);
	}
	for (int n = 0; n < 5; n++)
	{
		indices[ofsI+0 + 3 * n] = ofsV+0; indices[ofsI + 1 + 3 * n] = ofsV+n + 1; indices[ofsI + 2+3*n] =ofsV+n+2;
	}
	indices[ofsI + 0 + 3 * 5] = ofsV+0; indices[ofsI + 1 + 3 * 5] = ofsV+6; indices[ofsI + 2 + 3 * 5] = ofsV+1;

	ofsV += 7;		ofsI += 18;

	//下面

	vertices[ofsV + 0].Position = DirectX::XMFLOAT3(0, -0.5f, 0);   //中心の点

	for (int n = 0; n < 6; n++)
	{
		vertices[ofsV + 1 + n].Position = DirectX::XMFLOAT3(sin(DANGLE * n), -0.5f, cos(DANGLE * n));
	}

	for (int n = 0; n < 7; n++) {
		vertices[ofsV + n].normal = DirectX::XMFLOAT3(0, -0.5f, 0);
	}
	for (int n = 0; n < 5; n++)
	{
		indices[ofsI + 0 + 3 * n] = ofsV + 0; indices[ofsI + 1 + 3 * n] = ofsV + n + 2; indices[ofsI + 2 + 3 * n] = ofsV + n + 1;
	}
	indices[ofsI + 0 + 3 * 5] = ofsV + 0; indices[ofsI + 1 + 3 * 5] = ofsV + 1; indices[ofsI + 2 + 3 * 5] = ofsV + 6;

	ofsV += 7;		ofsI += 18;

	//側面
	vertices[ofsV + 0].Position = vertices[0 + 1].Position;
	vertices[ofsV + 1].Position = vertices[0 + 2].Position;
	vertices[ofsV + 2].Position = vertices[7 + 1].Position;
	vertices[ofsV + 3].Position = vertices[7 + 2].Position;

	for (int n = 0; n < 4; n++) {
		vertices[ofsV + n].normal = DirectX::XMFLOAT3(vertices[ofsV + n].Position.x,
													  0.0f,
												      vertices[ofsV + n].Position.z);
	}	
	
		indices[ofsI + 0] = ofsV + 0; indices[ofsI + 1] = ofsV + 1; indices[ofsI + 2] = ofsV + 1;
		indices[ofsI + 3] = ofsV + 1; indices[ofsI + 4] = ofsV + 2; indices[ofsI + 5] = ofsV + 3;

	
		ofsV += 4;		ofsI += 3 * 2;

	/*vertices[ofsV + 0].normal = DirectX::XMFLOAT3(vertices[ofsV+0].Position.x,
												   0.0f,
												   vertices[ofsV+0].Position.z);
	vertices[ofsV + 1].normal = DirectX::XMFLOAT3(vertices[ofsV+1].Position.x,
												   0.0f,
												  vertices[ofsV+2].Position.z);
	vertices[ofsV + 2].normal = DirectX::XMFLOAT3();
	vertices[ofsV + 3].normal = DirectX::XMFLOAT3();*/

	//BufferEdit(Device, vertices, ofsV, indices, ofsI);

	

}