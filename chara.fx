// �v���O����������󂯎����
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
//	�e�N�X�`��
Texture2D DiffuseTexture : register(t0);
SamplerState DecalSampler : register(s0);

//	���͒��_
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
//	���_�V�F�[�_�[
//--------------------------------------------
VStoPS VSMain(APPtoVS input)
{
	VStoPS output = (VStoPS)0;
	//	�X�N���[�����W�ϊ�
	float4 P = float4(input.Position, 1.0);
	output.Position = mul(matWVP, P);
	//	�@�����[���h�ϊ�
	float4 N = float4(input.Normal, 0.0);
	output.Normal = mul(World, N).xyz;

	output.Tex = input.Tex;
	return output;
}

//--------------------------------------------
//	�s�N�Z���V�F�[�_�[
//--------------------------------------------
float4 PSMain(VStoPS input) : SV_TARGET0
{
	float4 tex = DiffuseTexture.Sample(DecalSampler, input.Tex);
	//	����
	float3 Ambient = 0;
	float3 N = normalize(input.Normal);
	// �����FIBL(ImageBasedLighting)
	// ����F�������C�e�B���O
	float skyRate = (N.y + 1.0) * 0.5;	//max(0, N.y);
	Ambient += float3(0.5, 0.7, 0.8)*skyRate;
	float groundRate = (-N.y + 1)*0.5;	//max(0, -N.y);
	Ambient += float3(0.4, 0.3, 0.1)*groundRate;

	float3 Light = 0;
	//	���s��
	float3 L = float3(1.0, -1.0, -1.0);
	L = normalize(L);
	float rate = -dot(L,N);
	rate = max(0, rate);
	Light += float3(1.0, 1.0, 0.9)*rate;
	
	//	���C�g�K�p
	tex.rgb *= Light+Ambient;

	return tex;
}