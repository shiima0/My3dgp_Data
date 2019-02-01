// プログラム側から受け取る情報
cbuffer CBPerMesh : register(b0)
{
	matrix  World;
	matrix	View;
	matrix	Proj;
	matrix	matWVP;
	float	time;
	float	param1;
	float	param2;
	float	param3;
};
//	テクスチャ
Texture2D DiffuseTexture : register(t0);
SamplerState DecalSampler : register(s0);

//	入力頂点
struct APPtoVS {
	float3 Position : POSITION;
	float3 Normal   : NORMAL;
	float2 Tex      : TEXCOORD;
};

struct VStoPS
{
	float4 Position : SV_POSITION;
	float2 Tex : TEXCOORD0;
	float3 Normal : NORMAL;
	float3 WorldPos : TEXCOORD2;
};

//--------------------------------------------
//	頂点シェーダー
//--------------------------------------------
VStoPS VSMain(APPtoVS input)
{
	VStoPS output = (VStoPS)0;
	//	スクリーン座標変換
	float4 P = float4(input.Position, 1.0);
	output.Position = mul(matWVP, P);
	//	法線ワールド変換
	float4 N = float4(input.Normal, 0.0);
	output.Normal = mul(World, N).xyz;

	output.Tex = input.Tex;
	return output;
}

//--------------------------------------------
//	ピクセルシェーダー
//--------------------------------------------
float4 PSMain(VStoPS input) : SV_TARGET0
{
	float4 tex = DiffuseTexture.Sample(DecalSampler, input.Tex);
	//	環境光
	float3 Ambient = 0;
	float3 N = normalize(input.Normal);
	// 今風：IBL(ImageBasedLighting)
	// 無難：半球ライティング
	float skyRate = (N.y + 1.0) * 0.5;	//max(0, N.y);
	Ambient += float3(0.5, 0.7, 0.8)*skyRate;
	float groundRate = (-N.y + 1)*0.5;	//max(0, -N.y);
	Ambient += float3(0.4, 0.3, 0.1)*groundRate;

	float3 Light = 0;
	//	平行光
	float3 L = float3(1.0, -1.0, -1.0);
	L = normalize(L);
	float rate = -dot(L,N);
	rate = max(0, rate);
	Light += float3(1.0, 1.0, 0.9)*rate;
	
	//	ライト適用
	tex.rgb *= Light+Ambient;

	return tex;
}