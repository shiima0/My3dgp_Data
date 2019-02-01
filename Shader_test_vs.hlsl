#include"Shader_test.hlsli"
//--------------------------------------------
//	頂点シェーダー
//--------------------------------------------

PSInput main(VSInput input)
{
	PSInput output = (PSInput)0;
	
	//input.Normal.w = 0;
	//float4 N = normalize(mul(input.Normal, world));
	//float4 L = normalize(input.-light_direction);

	//UNIT21
	float3 p = { 0,0,0 };
	float3 n = { 0,0,0 };
	int index_bone = 0;
	for (index_bone = 0; index_bone < 4; index_bone++) {
		p += (input.bone_weights[index_bone] * mul(float4(input.Position.xyz, 1.0f), bone_transforms[input.bone_indices[index_bone]])).xyz;
		n += (input.bone_weights[index_bone] * mul(float4(input.Normal.xyz, 0), bone_transforms[input.bone_indices[index_bone]])).xyz;
	}

	input.Position = float4(p,1.0f);
	input.Normal =   float4(n,0.0f);

	


	//p = float4(input.Position, 1.0);
	// ワールド空間に変換.
	float4 worldPos = mul(p, World_view_projection);

	output.Position = worldPos;

	//output.ViewVec =
		//worldPos.xyz - float3(0, 4, 20);


	// 射影空間に変換.
	//float4 projPos = mul(p, matWVP);

	// 出力値設定.
	//output.Position = projPos;
	input.Color = material_color;
	input.Color.a = material_color.a;
	//output.Color = float4(1, 1, 1, 1);
	output.Tex = input.Tex;
	//	ライト計算
	//	float3 N;
	//	float3x3 m = (float3x3)matWVP;
	//	N = mul(m, N);

	/*float4 N =
		float4(input.Normal, 0);
	n =float4(input.Normal, 0);*/
	//n = mul(World, n);
	//n = normalize(n);
	//output.Normal = n.xyz;




	//	float3 L = float3(1, 0, 0);
	//	float I = -dot(N.xyz, L);
	//	I = max(0, I);	//0以下を0に
	//output.Color.rgb = I;
	return output;

}