//#include"Shader_test.hlsli"
#include"skinnd_mesh.hlsli"

Texture2D DiffuseTexture : register(t0);
//Texture2D EnvironmentTexture : register(t1);
Texture2D EffectTexture : register(t2);
SamplerState DecalSampler : register(s0);

//--------------------------------------------
//	�s�N�Z���V�F�[�_�[
//--------------------------------------------
float4 main(VS_OUT  pin ) : SV_TARGET{
	float4 tex = DiffuseTexture.Sample(DecalSampler, pin.texcoord);
	//	����
	float3 Ambient = 0;
	float3 N = normalize(pin.Normal);
	// �����FIBL(ImageBasedLighting)
	// ����F�������C�e�B���O
	float skyRate = (N.y + 1.0) * 0.5;	//max(0, N.y);
	Ambient += float3(0.5, 0.7, 0.8)*skyRate;
	float groundRate = (-N.y + 1)*0.5;	//max(0, -N.y);
	Ambient += float3(0.4, 0.3, 0.1)*groundRate;

	float3 Light = 0;
	//	���s��
	float3 L = float3(1.0, -1.0, -1.0);
//	float3 L = light_direction.xyz;
	L = normalize(L);
	float rate = -dot(L,N);
	rate = max(0, rate);
	Light += float3(1.0, 1.0, 0.9)*rate;

	//	���C�g�K�p
	tex.rgb *= Light + Ambient;

	return tex;


}
