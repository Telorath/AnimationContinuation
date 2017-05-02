#include "SharedDefines.h"
// Per-pixel color data passed through the pixel shader.
struct PixelShaderInput
{
	float4 pos : SV_POSITION;
	float3 uv : UV;
	float3 normal : NORMAL;
	float3 tangent: TANGENT;
	float3 bitangent: BITANGENT;
	float4 localpos : POSITION;
	float4 worldpos : POSITION_WORLD;
	float3 viewdir  : VIEWDIR;

};

Texture2D mesh : register(t0);

Texture2D meshNormal : register (t1);
Texture2D meshSpecular : register (t2);
SamplerState meshFilter : register(s0);



// A pass-through function for the (interpolated) color data.
float4 main(PixelShaderInput input) : SV_TARGET
{
	float3 newNormal = meshNormal.Sample(meshFilter, input.uv.xy).xyz;
	newNormal = (newNormal * 2.0f) - 1.0f;
	
	input.normal = normalize(input.normal);
	input.tangent = normalize(input.tangent);
	input.bitangent = normalize(input.bitangent);
	float4 dirpos = normalize(dirLight.pos);
	float3x3 TBN;
	TBN[0] = input.tangent.xyz;
	TBN[1] = input.bitangent.xyz;
	TBN[2] = input.normal.xyz;

	newNormal = mul(newNormal, TBN);
//Ambient Light
float4 Ambient = { .2f, .2f, .2f, .2f };
//Calculate Directional Light Ratio
float dirLightRatio = saturate(dot(-dirpos.xyz, newNormal));
//Compute directional light color
float4 dirFinalColor = dirLightRatio * dirLight.color;

////Calculate Point Light distance
//float3 dist = normalize(ptLight.pos - input.worldpos);
////Calculate Point Light Ratio
//float ptLightRatio = saturate(dot(dist, input.normal));
////Calculate Point Light Magnitude/Attenuation
//float Magnitude = saturate(length(ptLight.pos - input.worldpos) / ptLight.radius.x);
//float Attenuation = 1.0f - Magnitude;
//Attenuation *= Attenuation;
////Compute final Point Light Color
//float3 ptFinalColor = (ptLightRatio* Attenuation) * ptLight.color;
//
////Compute Spot Light Distance
//float3 dist_spot = normalize(spLight.pos - input.worldpos);
////Calculate Surface Ratio
//float spSurfaceRatio = saturate(dot(-dist_spot, normalize(spLight.dir)));
////Calculate Spot Factor
//float spotFactor = (spSurfaceRatio > spLight.outerRatio.x) ? 1 : 0;
////Calculate Spot Light Ratio
//float spLightRatio = saturate(dot(dist_spot, input.normal));
//float SpotMagnitude = saturate(length(spLight.pos - input.worldpos) / ptLight.radius.x);
//
////Calculate Attenuation
//float SpotAttenuation = 1.0f - saturate((spLight.innerRatio.x - spSurfaceRatio) / (spLight.innerRatio.x - spLight.outerRatio.x));
//SpotAttenuation *= SpotMagnitude;
////Compute final SpotLight Color
//float3 spFinalColor = (spLightRatio * SpotAttenuation) * spLight.color * spotFactor;

//Compute half vector
float3 halfVector = normalize((-dirpos.xyz) + input.viewdir.xyz);
float specularPow = meshSpecular.Sample(meshFilter, input.uv.xy).x;
float4 Intensity = max(pow(saturate(dot(newNormal, normalize(halfVector))),specularPow), 0);

//compute final light color
float4 FinalColor = dirFinalColor * Intensity * 1.0f/* + ptFinalColor + spFinalColor*/;
FinalColor = FinalColor + Ambient;
FinalColor = saturate(FinalColor);

float4 Final = mesh.Sample(meshFilter, (float2(input.uv.x, input.uv.y)));
if (Final.a < 0.1f)
	discard;
//compute and return the final pixel color

return (Final * FinalColor);
}
