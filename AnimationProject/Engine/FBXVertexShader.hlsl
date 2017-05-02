// A constant buffer that stores the three basic column-major matrices for composing geometry.

#pragma pack_matrix(row_major)

cbuffer ModelViewProjectionConstantBuffer : register(b0)
{
	matrix model;
	matrix view;
	matrix projection;
};

// Per-vertex data used as input to the vertex shader.
struct VertexShaderInput
{
	float3 pos : POSITION;
	float3 uv : UV;
	float3 norm : NORMAL;
};

// Per-pixel color data passed through the pixel shader.
struct PixelShaderInput
{
	float4 pos : SV_POSITION;
	float3 uv : UV;
	float3 normal : NORMAL;
	float4 localpos : POSITION;
	float4 worldpos : POSITION_WORLD;
};

// Simple shader to do vertex processing on the GPU.
PixelShaderInput main(VertexShaderInput input)
{
	PixelShaderInput output;
	float4 pos = float4(input.pos, 1.0f);

	// Transform the vertex position into projected space.
	output.localpos = pos;
	pos = mul(pos, model);
	output.worldpos = pos;
	pos = mul(pos, view);
	pos = mul(pos, projection);
	output.pos = pos;

	// Pass the color through without modification.
	output.uv = input.uv;

	//Calculate the normals into world space
	output.normal = mul(input.norm, (float3x3)(model));
	//Normalize
	//output.normal = normalize(output.normal);

	return output;
}
