
//--------------------------------------------
//	テクスチャ
//--------------------------------------------
Texture2D DiffuseTexture : register(t0);
//Texture2D EnvironmentTexture : register(t1);
Texture2D EffectTexture : register(t2);
SamplerState DecalSampler : register(s0);


//--------------------------------------------
//	グローバル変数
//--------------------------------------------
#define MAX_BONES (32)
cbuffer CBPerMesh : register(b0)
{
	row_major float4x4  World_view_projection;
	row_major float4x4	world;
	float4 material_color;
	float4 light_direction;
	//row_major float4x4	Proj;
	//matrix	matWVP;
	//float	time;
	row_major float4x4 bone_transforms[MAX_BONES];

	

};



//--------------------------------------------
//	データーフォーマット
//--------------------------------------------

struct VSInput
{
	float3 Position : POSITION;
	float4 Color    : COLOR;
	float4 Normal   : NORMAL;
	float2 Tex      : TEXCOORD;
	
	float4 bone_weights :WEIGHTS;
	uint4 bone_indices : BONES;
};

struct PSInput
{
	float4 Position : SV_POSITION;
	float4 Color : COLOR;
	float2 Tex : TEXCOORD;
	
	float3 Normal : NORMAL;
	float3 ViewVec : TEXCOORD1;
	
};

struct VS_OUT
{
	float4 position: SV_POSITION;
	float4 color:    COLOR;
	float2 texcoord :TEXCOORD;

	float3 Normal :  NORMAL;


};


