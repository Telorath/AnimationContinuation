#pragma once
#include <d3d11.h>
#include <Windows.h>
#include <string>
#include <map>
#include <vector>
//Temporary include until this can be abstracted out
#include <DirectXMath.h>
#include "Bone.h"
#include "RenderingComponent.h"

#define MaxBones 120

namespace Rendering
{

	class RenderingComponent;

	void SetBackBufferDimensions(unsigned int Width, unsigned int Height);

	//temporary structs
	struct DirectionalLight
	{
		DirectX::XMFLOAT4 Direction;
		DirectX::XMFLOAT4 Color;
	};
	struct ModelViewProjection
	{
		DirectX::XMFLOAT4X4 Model;
		DirectX::XMFLOAT4X4 View;
		DirectX::XMFLOAT4X4 Projection;
	};

	struct VertexPositionUVNormal
	{
		DirectX::XMFLOAT3 Position;
		DirectX::XMFLOAT3 UV;
		DirectX::XMFLOAT3 Normal;
	};

	struct VertexPositionUVNormalTangentBlendWeightBoneIndices
	{
		DirectX::XMFLOAT3 Position;
		DirectX::XMFLOAT3 UV;
		DirectX::XMFLOAT3 Normal;
		DirectX::XMFLOAT3 Tangent;
		DirectX::XMFLOAT4 BoneWeights;
		DirectX::XMUINT4 BoneIndices;
	};

	struct BoneMatrixBuffer
	{
		DirectX::XMFLOAT4X4 Bones[MaxBones];
	};

	struct VertexPositionUVBlendWeightBoneIndices
	{
		DirectX::XMFLOAT3 Position;
		DirectX::XMFLOAT3 UV;
		DirectX::XMFLOAT3 Normal;
		DirectX::XMFLOAT4 BoneWeights;
		DirectX::XMUINT4 BoneIndices;
	};
	//This class exists to manage the device and swap chain, as well as constant buffers used for rendering.
	class DXManager
	{
		ID3D11Device* m_Device;
		ID3D11DeviceContext* m_DeviceContext;
		IDXGISwapChain* m_SwapChain;
		ID3D11RenderTargetView* m_RenderTarget;
		ID3D11DepthStencilView* m_DepthStencil;
		D3D11_VIEWPORT m_Viewport;
//A buffer to carry light data
		ID3D11Buffer* m_LightingConstantBuffer;
		//temporary home of all renderable components.
		std::vector<RenderingComponent> m_RenderList;
		//Pointer to the camera's matrices
		DirectX::XMFLOAT4 m_DirLightDir;
		float XRot;
		float YRot;
		DirectX::XMFLOAT4X4* m_CameraWorld;
		DirectX::XMFLOAT4X4* m_CameraView;
		DirectX::XMFLOAT4X4* m_CameraProjection;
		ModelViewProjection m_MVPStruct;
		BoneMatrixBuffer m_BoneMatrixStruct;
		BoneMatrixBuffer m_BindPoseStruct;
		ID3D11Buffer* m_MVPConstantBuffer;
		ID3D11Buffer* m_BoneMatrixConstantBuffer;
		ID3D11Buffer* m_BindMatrixConstantBuffer;
		ID3D11Buffer* m_CameraPosConstantBuffer;
		HWND m_Window;

		void StaticRender(RenderingComponent* Renderable);
		void AnimatedRender(RenderingComponent* Renderable);
		void AdvancedAnimatedRender(RenderingComponent * Renderable);
	public:
		void init();
		void update();
		void shutdown();
		void SetWindow(HWND _Window);
		ID3D11DeviceContext* GetDeviceContext();
		ID3D11Device* GetDevice();
		void RegisterVertexShader(std::string Name, ID3D11VertexShader* Shader);
		void RegisterInputLayout(std::string Name, ID3D11InputLayout* InputLayout);
		void RegisterPixelShader(std::string Name, ID3D11PixelShader* Shader);
		void RegisterBuffer(std::string Name, ID3D11Buffer* Buffer);
		void RegisterIndexBuffer(std::string Name, unsigned int IndexCount, ID3D11Buffer* Buffer);
		void RegisterTexture2D(std::string Name, ID3D11Texture2D* Texture2D);
		void RegisterShaderResourceView(std::string Name, ID3D11ShaderResourceView* SRV);
		void CreateRenderable(std::string VertexShader, std::string InputLayout, std::string PixelShader, std::string VertexBuffer, std::string IndexBuffer, std::string Texture,DirectX::XMFLOAT4X4* World, std::vector<EngineAnimation::Bone>* BoneVector = nullptr, std::string SpecularMap = std::string(), std::string NormalMap = std::string());
		void SetViewProjection(DirectX::XMFLOAT4X4* CameraView, DirectX::XMFLOAT4X4* CameraProjection);
		void SetCameraPosition(DirectX::XMFLOAT4X4* CameraWorld);
	};
	//To be moved and fleshed out better later.
	class AssetManager
	{
		std::map<std::string, ID3D11VertexShader*> VertexShaderMap;
		std::map<std::string, ID3D11InputLayout*> InputLayoutMap;
		std::map<std::string, ID3D11PixelShader*> PixelShaderMap;
		std::map<std::string, ID3D11Buffer*> BufferMap;
		std::map<std::string, ID3D11Texture2D*> Texture2DMap;
		std::map<std::string, ID3D11ShaderResourceView*> ShaderResourceViewMap;
		std::map<std::string, unsigned int> BufferIndexCountMap;
		void CleanVertexShaderMap();
		void CleanInputLayoutMap();
		void CleanPixelShaderMap();
		void CleanBufferMap();
		void CleanTexture2DMap();
		void CleanShaderResourceViewMap();
		void CleanBufferIndexCountMap();
		void CleanAllMaps();
	public:
		//Returns a vertex shader by name.
		ID3D11VertexShader*			GetVertexShader(std::string Filename);
		//Returns an input layout by name
		ID3D11InputLayout*			GetInputLayout(std::string StructureName);
		//returns a pixel shader by name.
		ID3D11PixelShader*			GetPixelShader(std::string Filename);
		//Returns a buffer by name.
		ID3D11Buffer*				GetBuffer(std::string Buffername);
		//returns a shader resource view by name.
		ID3D11ShaderResourceView*	GetShaderResourceView(std::string Filename);
		//returns an unsigned int representing index count for an accompanying buffer. Name must have matching buffer in the buffer map.
		unsigned int GetIndexCount(std::string Buffername);
		//Cleans up all dynamic memory
		void shutdown();
		
		void RegisterVertexShader(std::string Name, ID3D11VertexShader* Shader);
		void RegisterInputLayout(std::string Name, ID3D11InputLayout* InputLayout);
		void RegisterPixelShader(std::string Name, ID3D11PixelShader* Shader);
		void RegisterBuffer(std::string Name, unsigned int VertexCount, ID3D11Buffer* Buffer);
		void RegisterTexture2D(std::string Name, ID3D11Texture2D* Texture);
		void RegisterShaderResourceView(std::string Name, ID3D11ShaderResourceView* SRV);

	};
	//To be moved and fleshed out better later
	AssetManager* GetAssetManager();
}