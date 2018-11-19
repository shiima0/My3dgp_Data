#include"skinnd_mesh.hlsli"
//UNIT 17
texture2D diffuse_map:register(t0);
SamplerState diffuse_map_sampler_state : register(s0);
//END ADD UNIT 17
float4 main(VS_OUT pin) : SV_TARGET
{
	//return pin.color;

	//UNIT 17
	return diffuse_map.Sample(diffuse_map_sampler_state, pin.texcoord)*pin.color;
}