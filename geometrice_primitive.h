#pragma once
#include <d3d11.h>
#include<DirectXMath.h>




class geometric_primitive {
protected:
	ID3D11VertexShader* Vertex;
	ID3D11PixelShader*  Pixel;
	ID3D11InputLayout*  Layout;
	ID3D11Buffer*       Buffer;
	ID3D11Buffer*       IBuffer;
	ID3D11Buffer*       CBuffer;
	ID3D11RasterizerState*RSSWireframe;
	ID3D11RasterizerState* RSSsolid;
	ID3D11DepthStencilState* Depth;
public:

	geometric_primitive(ID3D11Device*Device);
	virtual~geometric_primitive();

	struct vertex
	{
		DirectX::XMFLOAT3 Position;  //頂点
		DirectX::XMFLOAT3 normal;	 //法線
	};


	void BufferEdit(ID3D11Device*Device,
						vertex*vertices,
								  int T,
						u_int* indeices,
						int numIndex
				   );
	void render(ID3D11DeviceContext* Context,			//デバイスコンテキスト
		const DirectX::XMFLOAT4X4& word_view,			//ワールド・ビュー・プロジェクション合成行列
		const DirectX::XMFLOAT4X4& wordM,				//ワールド変換行列
		const DirectX::XMFLOAT4& light,					//ライト進行方向
		const DirectX::XMFLOAT4& Material_color,		//素材質色
		bool  bWareframe);								//線・塗りつぶし描画フラグ
	

	
	struct cbuffer
	{
		DirectX::XMFLOAT4X4 world_view_projection;  //ワールド・ビュー・プロジェクション合成行列
		DirectX::XMFLOAT4X4 world;					//ワールド変換行列
		DirectX::XMFLOAT4   material_color;			//材質色
		DirectX::XMFLOAT4 light_direction;			//ライト進行方向

	};

};
//立方体クラス
class geometric_Cube : public geometric_primitive
{
public:
			geometric_Cube(ID3D11Device*Device);
			~geometric_Cube() {};

			void Release() {}

};
//円柱
class geometric_Cylinder : public geometric_primitive
{
public:
	const int NUM;
	geometric_Cylinder(ID3D11Device*Device, int num=6);
	~geometric_Cylinder() {};

	
};
class  geometric_Sphere : public geometric_primitive
{};
class  geometric_Capsule : public geometric_primitive
{};


