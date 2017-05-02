// A constant buffer that stores the three basic column-major matrices for composing geometry.

#pragma pack_matrix(row_major)

#define MaxBones 120

cbuffer ModelViewProjectionConstantBuffer : register(b0)
{
	matrix model;
	matrix view;
	matrix projection;
};

cbuffer BoneMatrices : register(b1)
{
	matrix Bones[MaxBones];
}

cbuffer CameraBuffer : register(b2)
{
	float4 cameraPosition;
};

// Per-vertex data used as input to the vertex shader.
struct VertexShaderInput
{
	float3 pos : POSITION;
	float3 uv : UV;
	float3 norm : NORMAL;
	float3 tan: TANGENT;
	float4 boneweights : BLENDWEIGHTS;
	uint4 boneindices : BLENDINDICES;
};

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

// Simple shader to do vertex processing on the GPU.
PixelShaderInput main(VertexShaderInput input)
{
	PixelShaderInput output;
	float4 pos = float4(input.pos, 1.0f);

	float4 bindpos = pos;

	// Transform the vertex position into projected space.
	output.localpos = pos;
	//pos = mul(pos, model);
	pos = mul(bindpos, Bones[input.boneindices[0]] * input.boneweights[0]);
	pos = pos + mul(bindpos, Bones[input.boneindices[1]] * input.boneweights[1]);
	pos = pos + mul(bindpos, Bones[input.boneindices[2]] * input.boneweights[2]);
	pos = pos + mul(bindpos, Bones[input.boneindices[3]] * input.boneweights[3]);
		//pos.w = 1.0f;
	output.worldpos = pos;
	pos = mul(pos, view);
	pos = mul(pos, projection);
	output.pos = pos;

	// Pass the color through without modification.
	output.uv = input.uv;

	//Calculate the normals into world space
	float4 norm = float4(input.norm, 1);
	norm = mul(norm, Bones[input.boneindices[0]] * input.boneweights[0]);
	norm = norm + mul(norm, Bones[input.boneindices[1]] * input.boneweights[1]);
	norm = norm + mul(norm, Bones[input.boneindices[2]] * input.boneweights[2]);
	norm = norm + mul(norm, Bones[input.boneindices[3]] * input.boneweights[3]);
	output.normal = norm.xyz;
	//Normalize
	//output.normal = normalize(output.normal);

	//Calculate the tangent
	float4 tan = float4(input.tan,1);
	tan = mul(tan, Bones[input.boneindices[0]] * input.boneweights[0]);
	tan = tan + mul(tan, Bones[input.boneindices[1]] * input.boneweights[1]);
	tan = tan + mul(tan, Bones[input.boneindices[2]] * input.boneweights[2]);
	tan = tan + mul(tan, Bones[input.boneindices[3]] * input.boneweights[3]);
	output.tangent = tan.xyz;
	//output.tangent = mul(input.tan, (float3x3)(model));
	output.bitangent = mul(cross(norm.xyz, tan.xyz), (float3x3)(model));

//calculate the view direction
	output.viewdir = cameraPosition.xyz - output.worldpos.xyz;
	output.viewdir = normalize(output.viewdir);

	return output;

}