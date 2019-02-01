#include"skinnd_mesh.hlsli"

Texture2D DiffuseTexture : register(t0);
Texture2D EnvironmentTexture : register(t1);
SamplerState DecalSampler : register(s0);
float4 main(VS_OUT pin) : SV_TARGET
{
	float4 albedo = 1;
	float3 N =
	normalize(pin.Normal);

	float3 L = normalize(
		float3(1, -1, 0));
	float I = -dot(N.xyz, L);
	//I = max(0, I); //0�ȉ���0��
	I = (I + 1.0) * 0.5;

	float3 LColor = 0;

	LColor += float3(1.0, 0.95, 0.9)*I;
	// ��̌�
	float skyrate = (N.y + 1.0)*0.5;
	LColor +=
		float3(0.5, 0.7, 0.8)*skyrate;
	// �n�ʂ̌�
	float groundrate = (-N.y + 1.0)*0.5;
	LColor +=
		float3(0.05, 0.2, 0.0)*groundrate;

	float3 V = normalize(
		pin.ViewVec);
	float3 R = reflect(V, N); //���˂��v�Z����֐�
	float2 env_uv;
	env_uv.y = R.y * (-0.5) + 0.5;
	env_uv.x = R.x * 0.25 + 0.25;
	float4 reflectivity = EnvironmentTexture.
	Sample(DecalSampler,
		env_uv);
	
	pin.color = albedo*float4(0.3,0.2,0.15,1);
	pin.color += reflectivity*float4(0.5, 0.4, 0.3, 1); //�v���������F
												//color += reflectivity*float4(0.543, 0.858, 0.576, 1); //�N�����v���������F
	//pin.color.rgb *= LColor;

	return pin.color;
}