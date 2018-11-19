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
	//右手系の座標軸を左手系の座標軸に変換する
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
		DirectX::XMFLOAT4X4 world_view_projection;  //ワールド・ビュー・プロジェクション合成行列
		DirectX::XMFLOAT4X4 world;					//ワールド変換行列
		DirectX::XMFLOAT4   material_color;			//材質色
		DirectX::XMFLOAT4 light_direction;			//ライト進行方向
	
		DirectX::XMFLOAT4X4 bone_transforms[MAX_BONES]; //ボーン影響度

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

		//追加
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

	void render(ID3D11DeviceContext* Context,			//デバイスコンテキスト
		const DirectX::XMFLOAT4X4& word_view,			//ワールド・ビュー・プロジェクション合成行列
		const DirectX::XMFLOAT4X4& wordM,				//ワールド変換行列
		const DirectX::XMFLOAT4& light,					//ライト進行方向
		const DirectX::XMFLOAT4& Material_color,		//素材質色
		bool  bWareframe,								//線・塗りつぶし描画フラグ
		float elapsed_time/*UNIT23*/
		);	
private:
	int m_indexCount;
	int m_vertexCount;
	int m_latitudeNum;
	int m_longitudeNum;
};