#include "RenderingComponent.h"
#include "Rendering.h"

namespace Rendering
{

	ID3D11PixelShader * RenderingComponent::GetPixelShader()
	{
		return PixelShader;
	}

	ID3D11VertexShader * RenderingComponent::GetVertexShader()
	{
		return VertexShader;
	}

	ID3D11InputLayout * RenderingComponent::GetInputLayout()
	{
		return InputLayout;
	}

	ID3D11Buffer * RenderingComponent::GetVertexBuffer()
	{
		return VertexBuffer;
	}

	ID3D11Buffer * RenderingComponent::GetIndexBuffer()
	{
		return IndexBuffer;
	}

	ID3D11ShaderResourceView* RenderingComponent::GetTexture()
	{
		return Texture;
	}

	ID3D11ShaderResourceView * RenderingComponent::GetSpecularMap()
	{
		return SpecularMap;
	}

	ID3D11ShaderResourceView * RenderingComponent::GetNormalMap()
	{
		return NormalMap;
	}

	unsigned int RenderingComponent::GetIndexCount()
	{
		return IndexCount;
	}

	DirectX::XMFLOAT4X4 * RenderingComponent::GetWorldMatrixPTR()
	{
		return WorldMatrix;
	}

	DirectX::XMFLOAT4X4 ** RenderingComponent::GetBoneMatricesPTRPTR()
	{
		return BoneMatrices;
	}

	DirectX::XMFLOAT4X4* RenderingComponent::GetBindPosePTR()
	{
		return &BindPose[0];
	}

	unsigned int RenderingComponent::GetNumBones()
	{
		return NumBones;
	}

	bool RenderingComponent::SetVertexShader(std::string VertexShaderName)
	{
		VertexShader = GetAssetManager()->GetVertexShader(VertexShaderName);
		return (!(VertexShader == nullptr));
	}

	bool RenderingComponent::SetInputLayout(std::string InputLayoutName)
	{
		InputLayout = GetAssetManager()->GetInputLayout(InputLayoutName);
		return (!(InputLayout == nullptr));
	}

	bool RenderingComponent::SetPixelShader(std::string PixelShaderName)
	{
		PixelShader = GetAssetManager()->GetPixelShader(PixelShaderName);
		return (!(PixelShader == nullptr));
	}

	bool RenderingComponent::SetIndexBuffer(std::string IndexBufferName)
	{
		IndexBuffer = GetAssetManager()->GetBuffer(IndexBufferName);
		IndexCount = GetAssetManager()->GetIndexCount(IndexBufferName);
		return (!(IndexBuffer == nullptr));
	}
	bool RenderingComponent::SetVertexBuffer(std::string VertexBufferName)
	{
		VertexBuffer = GetAssetManager()->GetBuffer(VertexBufferName);
		return (!(VertexBuffer == nullptr));
	}
	bool RenderingComponent::SetTexture(std::string TextureName)
	{
		Texture = GetAssetManager()->GetShaderResourceView(TextureName);
		return (!(Texture == nullptr));
	}
	bool RenderingComponent::SetSpecularMap(std::string TextureName)
	{
		SpecularMap = GetAssetManager()->GetShaderResourceView(TextureName);
		return (!(Texture == nullptr));
	}
	bool RenderingComponent::SetNormalMap(std::string TextureName)
	{
		NormalMap = GetAssetManager()->GetShaderResourceView(TextureName);
		return (!(Texture == nullptr));
	}
	void RenderingComponent::SetWorldMatrixPTR(DirectX::XMFLOAT4X4 * NewWorldMatrix)
	{
		WorldMatrix = NewWorldMatrix;
	}
	void RenderingComponent::SetBoneMatrixPTR(unsigned int Index, DirectX::XMFLOAT4X4 * NewBoneMatrix)
	{
		BoneMatrices[Index] = NewBoneMatrix;
	}
	void RenderingComponent::SetNumBones(unsigned int NewNumBones)
	{
		NumBones = NewNumBones;
	}
}