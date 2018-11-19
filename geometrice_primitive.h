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
		DirectX::XMFLOAT3 Position;  //���_
		DirectX::XMFLOAT3 normal;	 //�@��
	};


	void BufferEdit(ID3D11Device*Device,
						vertex*vertices,
								  int T,
						u_int* indeices,
						int numIndex
				   );
	void render(ID3D11DeviceContext* Context,			//�f�o�C�X�R���e�L�X�g
		const DirectX::XMFLOAT4X4& word_view,			//���[���h�E�r���[�E�v���W�F�N�V���������s��
		const DirectX::XMFLOAT4X4& wordM,				//���[���h�ϊ��s��
		const DirectX::XMFLOAT4& light,					//���C�g�i�s����
		const DirectX::XMFLOAT4& Material_color,		//�f�ގ��F
		bool  bWareframe);								//���E�h��Ԃ��`��t���O
	

	
	struct cbuffer
	{
		DirectX::XMFLOAT4X4 world_view_projection;  //���[���h�E�r���[�E�v���W�F�N�V���������s��
		DirectX::XMFLOAT4X4 world;					//���[���h�ϊ��s��
		DirectX::XMFLOAT4   material_color;			//�ގ��F
		DirectX::XMFLOAT4 light_direction;			//���C�g�i�s����

	};

};
//�����̃N���X
class geometric_Cube : public geometric_primitive
{
public:
			geometric_Cube(ID3D11Device*Device);
			~geometric_Cube() {};

			void Release() {}

};
//�~��
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


