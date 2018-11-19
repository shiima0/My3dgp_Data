#include <d3d11.h>
#include<DirectXMath.h>
#include <vector>


#define MAX_BONES (32)
class skinned_mesh {
protected:

	ID3D11VertexShader* Vertex;
	ID3D11PixelShader*  Pixel;
	ID3D11InputLayout*  Layout;
	//ID3D11Buffer*       Buffer;
	//ID3D11Buffer*       IBuffer;
	ID3D11Buffer*       CBuffer;
	ID3D11RasterizerState*RSSWireframe;
	ID3D11RasterizerState* RSSsolid;
	ID3D11DepthStencilState* Depth;
	ID3D11SamplerState*      SamplerDesc;

	ID3D11ShaderResourceView* SRV;
	D3D11_TEXTURE2D_DESC texture2d_desc;

	HRESULT hr;

	ID3D11Texture2D *texture2d;

	//UNIT21
	//convert coordinate system from 'UP:+Z FRONT:+Y RIGHT-HAND' to 'UP:+Y FRONT:+Z LEFT-HAND 
	//�E��n�̍��W��������n�̍��W���ɕϊ�����
	DirectX::XMFLOAT4X4 coordinate_conversion = {
		1,0,0,0,
		0,0,1,0,
		0,1,0,0,
		0,0,0,1,
	};

public:
	skinned_mesh(ID3D11Device*Device, const char *fbx_filename);
	~skinned_mesh();


	struct vertex
	{
		DirectX::XMFLOAT3 Position;
		DirectX::XMFLOAT4 color;
		DirectX::XMFLOAT4 normal;
		
		DirectX::XMFLOAT2 texcoord;
		


#define MAX_BONE_INFLUENCES 4
		FLOAT bone_weights[MAX_BONE_INFLUENCES] = { 1,0,0,0 };
		INT   bone_indices[MAX_BONE_INFLUENCES] = {};
	};


	struct cbuffer
	{
		DirectX::XMFLOAT4X4 world_view_projection;  //���[���h�E�r���[�E�v���W�F�N�V���������s��
		DirectX::XMFLOAT4X4 world;					//���[���h�ϊ��s��
		DirectX::XMFLOAT4   material_color;			//�ގ��F
		DirectX::XMFLOAT4 light_direction;			//���C�g�i�s����
	
		DirectX::XMFLOAT4X4 bone_transforms[MAX_BONES]; //�{�[���e���x

	};

	struct material
	{
		DirectX::XMFLOAT4 color = { 0.8f,0.8f,0.8f,1.0f };
		ID3D11ShaderResourceView*shader_resource_view;
	};
	//material diffuse;

	struct subset
	{
		u_int index_start = 0;	//start number of index buferr
		u_int index_count = 0;  //number of vertices


		material diffuse;
	};
	//std::vector<subset>subsets;

	//UNIT22
	struct bone
	{
		DirectX::XMFLOAT4X4 transform;
	};
	//UNIT23
	typedef std::vector<bone> skeletal;
	struct skeletal_animation : public std::vector<skeletal>
	{
		float sampling_time = 1 / 24.0f;
		float animation_tick = 0.0f;
	};
	
	struct mesh
	{
		ID3D11Buffer*vertex_buffer;
		ID3D11Buffer*index_buffer;
		std::vector<subset>subsets;

		//�ǉ�
		DirectX::XMFLOAT4X4 global_tramsform = { 1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1 };

		//UNIT22
		//std::vector<skinned_mesh::bone> skeletal;

		//UNIT23
		skeletal_animation skeletal_animation;
	};
	std::vector<mesh>meshes;

	


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
		bool  bWareframe,								//���E�h��Ԃ��`��t���O
		float elapsed_time/*UNIT23*/
		);	
private:
	int m_indexCount;
	int m_vertexCount;
	int m_latitudeNum;
	int m_longitudeNum;
};