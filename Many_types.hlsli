
struct VS_OUT
{
	float4 position: SV_POSITION;
	float4 color:    COLOR;
	float2 texcoord :TEXCOORD;

	float3 Normal :  NORMAL;


};
#define MAX_BONES (32)
cbuffer CONSTANT_BUFFER : register(b0)
{
	row_major float4x4 world_view_projection;
	row_major float4x4 world;
	float4 material_color;
	float4 light_direction;
	//UNIT21
	row_major float4x4 bone_transforms[MAX_BONES];

};