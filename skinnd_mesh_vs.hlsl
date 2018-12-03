#include"skinnd_mesh_ps.hlsl"
VS_OUT main(float4 position : POSITION, float4 normal : NORMAL, float2 texcoord : TEXCOORD, float4 bone_weights : WEIGHTS, uint4 bone_indices : BONES) //UNIT20追加ありテキストコードの後ろから分！
{
	VS_OUT vout;
	vout.position = mul(position, world_view_projection);

	normal.w = 0;
	float4 N = normalize(mul(normal, world));
	float4 L = normalize(-light_direction);

	//UNIT21
	float3 p = { 0,0,0 };
	float3 n = { 0,0,0 };
	int index_bone = 0;
	for (index_bone = 0; index_bone < 4; index_bone++) {
		p += (bone_weights[index_bone] * mul(position, bone_transforms[bone_indices[index_bone]])).xyz;
		n += (bone_weights[index_bone] * mul(float4(normal.xyz, 0), bone_transforms[bone_indices[index_bone]])).xyz;
	}

	position = float4(p, 1.0f);
	normal =   float4(n, 0.0f);

	//UNIT20
	float4 influence = { 0,0,0,1 };
	for (int i = 0; i < 4; i++) {
		float weight = bone_weights[i];
		if (weight > 0.0f) {
			switch (bone_indices[i])
			{
			case 0: influence.r = weight; break;
			case 1: influence.g = weight; break;
			case 2: influence.b = weight; break;
			default:break;
			}
		}
	}

	

	vout.position = mul(position, world_view_projection);

	
	//vout.color = influence;
	

	vout.color = material_color*max(0, dot(L, N));
	vout.color.a = material_color.a;

	//Unit 17
	vout.texcoord = texcoord;


	return vout;
}