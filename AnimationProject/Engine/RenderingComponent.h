#pragma once
#include <DirectXMath.h>
#include <d3d11.h>
#include "Bone.h"
#include <string>
//include "Rendering.h"
#define MaxBones 120
namespace Rendering
{
	class DXManager;
	class RenderingComponent;
	typedef void (DXManager::*RenderMethodFunctionPtr)(RenderingComponent*);

	class RenderingComponent
	{
		ID3D11VertexShader* VertexShader;
		ID3D11InputLayout* InputLayout;
		ID3D11PixelShader* PixelShader;
		ID3D11Buffer* VertexBuffer;
		ID3D11Buffer* IndexBuffer;
		unsigned int IndexCount;
		ID3D11ShaderResourceView* Texture;
		ID3D11ShaderResourceView* SpecularMap;
		ID3D11ShaderResourceView* NormalMap;
		DirectX::XMFLOAT4X4* WorldMatrix;
		DirectX::XMFLOAT4X4* BoneMatrices[MaxBones];
		DirectX::XMFLOAT4X4 BindPose[MaxBones];
		unsigned int NumBones;

	public:
		RenderMethodFunctionPtr RenderMethod;

		ID3D11PixelShader* GetPixelShader();
		ID3D11VertexShader* GetVertexShader();
		ID3D11InputLayout* GetInputLayout();
		ID3D11Buffer* GetVertexBuffer();
		ID3D11Buffer* GetIndexBuffer();
		ID3D11ShaderResourceView* GetTexture();
		ID3D11ShaderResourceView* GetSpecularMap();
		ID3D11ShaderResourceView* GetNormalMap();
		unsigned int GetIndexCount();
		unsigned int GetNumBones();
		DirectX::XMFLOAT4X4* GetWorldMatrixPTR();
		DirectX::XMFLOAT4X4** GetBoneMatricesPTRPTR();
		DirectX::XMFLOAT4X4* GetBindPosePTR();

		bool SetVertexShader(std::string VertexShaderName);
		bool SetInputLayout(std::string InputLayoutName);
		bool SetPixelShader(std::string PixelShaderName);
		bool SetIndexBuffer(std::string IndexBufferName);
		bool SetVertexBuffer(std::string VertexBufferName);
		bool SetTexture(std::string TextureName);
		bool SetSpecularMap(std::string TextureName);
		bool SetNormalMap(std::string TextureName);
		void SetWorldMatrixPTR(DirectX::XMFLOAT4X4* NewWorldMatrix);
		void SetBoneMatrixPTR(unsigned int Index, DirectX::XMFLOAT4X4* NewBoneMatrix);
		void SetNumBones(unsigned int NewNumBones);
	};
}