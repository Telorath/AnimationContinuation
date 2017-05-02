#include "Rendering.h"
#pragma comment(lib,"d3d11.lib")
#include <DirectXMath.h>
#include <map>
#include <string>
#include "SharedDefines.h"
#include "RenderingComponent.h"

namespace Rendering
{
	unsigned int BackBuffer_Width;

	unsigned int BackBuffer_Height;

	AssetManager MainAssetManager;

#pragma region DXManager

	void DXManager::init()
	{
#pragma region DeviceandSwapchain
		DXGI_SWAP_CHAIN_DESC SCDesc;
		ZeroMemory(&SCDesc, sizeof(SCDesc));
		SCDesc.Windowed = true;
		SCDesc.BufferCount = 1;
		SCDesc.BufferDesc.Width = BackBuffer_Width;
		SCDesc.BufferDesc.Height = BackBuffer_Height;
		SCDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		SCDesc.BufferDesc.RefreshRate.Numerator = 60;
		SCDesc.BufferDesc.RefreshRate.Denominator = 1;
		SCDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		SCDesc.OutputWindow = m_Window;
		SCDesc.SampleDesc.Count = 1;
		SCDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
		const D3D_FEATURE_LEVEL D3DFeatureLevelsRequested[] = { D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_10_0 };
		D3D_FEATURE_LEVEL D3DFeatureLevelsSupported;
		UINT NumLevels = 4;
		D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, D3DFeatureLevelsRequested, NumLevels, D3D11_SDK_VERSION, &SCDesc, &m_SwapChain, &m_Device, &D3DFeatureLevelsSupported, &m_DeviceContext);
#pragma endregion

#pragma region BackbufferSetup

		ID3D11Resource* BackBuffer;
		m_SwapChain->GetBuffer(0, __uuidof(BackBuffer), reinterpret_cast<void**>(&BackBuffer));
		m_Device->CreateRenderTargetView(BackBuffer, NULL, &m_RenderTarget);

		D3D11_TEXTURE2D_DESC TextureDesc;
		ZeroMemory(&TextureDesc, sizeof(D3D11_TEXTURE2D_DESC));
		TextureDesc.Width = BackBuffer_Width;
		TextureDesc.Height = BackBuffer_Height;
		TextureDesc.ArraySize = 1;
		TextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		TextureDesc.Format = DXGI_FORMAT_D32_FLOAT;
		TextureDesc.Usage = D3D11_USAGE_DEFAULT;
		TextureDesc.SampleDesc.Count = 1;
		TextureDesc.MipLevels = 0;

		ID3D11Texture2D* Stencil;

		m_Device->CreateTexture2D(&TextureDesc, nullptr, &Stencil);

		D3D11_DEPTH_STENCIL_VIEW_DESC DSVDesc;
		ZeroMemory(&DSVDesc, sizeof(DSVDesc));
		DSVDesc.Format = DXGI_FORMAT_D32_FLOAT;
		DSVDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

		m_Device->CreateDepthStencilView(Stencil, &DSVDesc, &m_DepthStencil);

		if (BackBuffer)
		{
			BackBuffer->Release();
			BackBuffer = nullptr;
		}

		DXGI_SWAP_CHAIN_DESC Desc;
		m_SwapChain->GetDesc(&Desc);
		ZeroMemory(&m_Viewport, sizeof(m_Viewport));
		m_Viewport.TopLeftX = 0;
		m_Viewport.TopLeftY = 0;
		m_Viewport.Width = (float)Desc.BufferDesc.Width;
		m_Viewport.Height = (float)Desc.BufferDesc.Height;
		m_Viewport.MinDepth = 0;
		m_Viewport.MaxDepth = 1;


		m_DeviceContext->OMSetRenderTargets(1, &m_RenderTarget, m_DepthStencil);

		//		m_DeviceContext->OM

		m_DeviceContext->RSSetViewports(1, &m_Viewport);

#pragma endregion

		//#pragma region Set Sampler State
		//		ID3D11SamplerState* samplerState;
		//
		//		D3D11_SAMPLER_DESC samplerDesc;
		//		ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));
		//		samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
		//		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		//		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		//		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		//		samplerDesc.MaxAnisotropy = 8;
		//		samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		//		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
		//		samplerDesc.MipLODBias = 0;
		//
		//		HRESULT result = m_Device->CreateSamplerState(&samplerDesc, &samplerState);
		//		m_DeviceContext->PSSetSamplers(0, 1, &samplerState);
		//
		//#pragma endregion

		dirLightCB ConstantBufferData;

		ConstantBufferData.dirLight.color = DirectX::XMFLOAT4{ 1.0f,1.0f,1.0f,1.0f };
		ConstantBufferData.dirLight.pos = DirectX::XMFLOAT4{ 0.0f,-1.0f,1.0f,0.0f };
		m_DirLightDir = DirectX::XMFLOAT4{ 0.0f,-1.0f,1.0f,0.0f };

		CD3D11_BUFFER_DESC LightconstantBufferDesc(sizeof(dirLightCB), D3D11_BIND_CONSTANT_BUFFER);

		m_Device->CreateBuffer(&LightconstantBufferDesc, nullptr, &m_LightingConstantBuffer);

		m_DeviceContext->UpdateSubresource(m_LightingConstantBuffer, 0, 0, &ConstantBufferData, 0, 0);
		m_DeviceContext->PSSetConstantBuffers(1, 1, &m_LightingConstantBuffer);

		CD3D11_BUFFER_DESC MVPconstantBufferDesc(sizeof(ModelViewProjection), D3D11_BIND_CONSTANT_BUFFER);

		m_Device->CreateBuffer(&MVPconstantBufferDesc, nullptr, &m_MVPConstantBuffer);

		CD3D11_BUFFER_DESC BoneconstantBufferDesc(sizeof(BoneMatrixBuffer), D3D11_BIND_CONSTANT_BUFFER);

		m_Device->CreateBuffer(&BoneconstantBufferDesc, nullptr, &m_BoneMatrixConstantBuffer);

		CD3D11_BUFFER_DESC CameraPositionConstantBufferDesc(sizeof(DirectX::XMFLOAT4), D3D11_BIND_CONSTANT_BUFFER);

		m_Device->CreateBuffer(&CameraPositionConstantBufferDesc, nullptr, &m_CameraPosConstantBuffer);

	}

	void DXManager::update()
	{
		//Clear the backbuffer
		float red[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

		m_DeviceContext->ClearRenderTargetView(m_RenderTarget, red);
		m_DeviceContext->ClearDepthStencilView(m_DepthStencil, D3D11_CLEAR_FLAG::D3D11_CLEAR_DEPTH | D3D11_CLEAR_FLAG::D3D11_CLEAR_STENCIL, 1, NULL);

		dirLightCB ConstantBufferData;
		ConstantBufferData.dirLight.color = DirectX::XMFLOAT4{ 1.0f,1.0f,1.0f,1.0f };
		ConstantBufferData.dirLight.pos = m_DirLightDir;

		
		m_DeviceContext->UpdateSubresource(m_LightingConstantBuffer, 0, 0, &ConstantBufferData, 0, 0);
		m_DeviceContext->PSSetConstantBuffers(1, 1, &m_LightingConstantBuffer);


		//Our View and Projection are set once per frame based on the camera.
		m_MVPStruct.View = *m_CameraView;
		m_MVPStruct.Projection = *m_CameraProjection;
		for (size_t i = 0; i < m_RenderList.size(); i++)
		{
#if true
			RenderingComponent* Render = &m_RenderList[i];
			((this)->*(Render->RenderMethod))(&m_RenderList[i]);
#endif
		}
		//Present the backbuffer
		m_SwapChain->Present(0, 0);
	}

	void DXManager::StaticRender(RenderingComponent* Renderable)
	{
		unsigned int Stride = sizeof(VertexPositionUVNormal);
		unsigned int Offset = 0;
		ID3D11Buffer* BufferPointer;
		BufferPointer = Renderable->GetVertexBuffer();

		m_DeviceContext->IASetVertexBuffers(0, 1, &BufferPointer, &Stride, &Offset);

		m_DeviceContext->IASetInputLayout(Renderable->GetInputLayout());

		BufferPointer = Renderable->GetIndexBuffer();

		m_DeviceContext->IASetIndexBuffer(BufferPointer, DXGI_FORMAT_R16_UINT, 0);

		//Update the constant buffer

		m_MVPStruct.Model = *Renderable->GetWorldMatrixPTR();

		m_DeviceContext->UpdateSubresource(m_MVPConstantBuffer, 0, NULL, &m_MVPStruct, 0, 0);

		ID3D11Buffer* Buffers[1]{ m_MVPConstantBuffer };

		m_DeviceContext->VSSetConstantBuffers(0, 1, Buffers);

		m_DeviceContext->VSSetShader(Renderable->GetVertexShader(), nullptr, 0);

		m_DeviceContext->PSSetShader(Renderable->GetPixelShader(), nullptr, 0);

		ID3D11ShaderResourceView* SRV = Renderable->GetTexture();

		m_DeviceContext->PSSetShaderResources(0, 0, &SRV);

		m_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		m_DeviceContext->DrawIndexed(Renderable->GetIndexCount(), 0, 0);
	}

	void DXManager::AnimatedRender(RenderingComponent * Renderable)
	{
		unsigned int Stride = sizeof(VertexPositionUVBlendWeightBoneIndices);
		unsigned int Offset = 0;
		ID3D11Buffer* BufferPointer;
		BufferPointer = Renderable->GetVertexBuffer();

		m_DeviceContext->IASetVertexBuffers(0, 1, &BufferPointer, &Stride, &Offset);

		m_DeviceContext->IASetInputLayout(Renderable->GetInputLayout());

		BufferPointer = Renderable->GetIndexBuffer();

		m_DeviceContext->IASetIndexBuffer(BufferPointer, DXGI_FORMAT_R16_UINT, 0);

		//Update the constant buffer

		m_MVPStruct.Model = *Renderable->GetWorldMatrixPTR();

		m_DeviceContext->UpdateSubresource(m_MVPConstantBuffer, 0, NULL, &m_MVPStruct, 0, 0);

		{
			using namespace DirectX;
			for (unsigned int i = 0; i < Renderable->GetNumBones(); i++)
			{
				XMMATRIX InverseBindPoseMatrix = XMLoadFloat4x4(&Renderable->GetBindPosePTR()[i]);
				XMMATRIX FrameBoneMatrix = XMLoadFloat4x4(Renderable->GetBoneMatricesPTRPTR()[i]);
				XMStoreFloat4x4(&m_BoneMatrixStruct.Bones[i], (InverseBindPoseMatrix * FrameBoneMatrix));
			}
		}
		m_DeviceContext->UpdateSubresource(m_BoneMatrixConstantBuffer, 0, NULL, &m_BoneMatrixStruct, 0, 0);

		ID3D11Buffer* Buffers[2]{ m_MVPConstantBuffer, m_BoneMatrixConstantBuffer };

		m_DeviceContext->VSSetConstantBuffers(0, 2, Buffers);

		m_DeviceContext->VSSetShader(Renderable->GetVertexShader(), nullptr, 0);

		m_DeviceContext->PSSetShader(Renderable->GetPixelShader(), nullptr, 0);

		ID3D11ShaderResourceView* SRV = Renderable->GetTexture();

		m_DeviceContext->PSSetShaderResources(0, 1, &SRV);

		m_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		m_DeviceContext->DrawIndexed(Renderable->GetIndexCount(), 0, 0);
	}

	void DXManager::AdvancedAnimatedRender(RenderingComponent* Renderable)
	{
		unsigned int Stride = sizeof(VertexPositionUVNormalTangentBlendWeightBoneIndices);
		unsigned int Offset = 0;
		ID3D11Buffer* BufferPointer;
		BufferPointer = Renderable->GetVertexBuffer();

		m_DeviceContext->IASetVertexBuffers(0, 1, &BufferPointer, &Stride, &Offset);

		m_DeviceContext->IASetInputLayout(Renderable->GetInputLayout());

		BufferPointer = Renderable->GetIndexBuffer();

		m_DeviceContext->IASetIndexBuffer(BufferPointer, DXGI_FORMAT_R32_UINT, 0);

		//Update the constant buffer

		m_MVPStruct.Model = *Renderable->GetWorldMatrixPTR();

		m_DeviceContext->UpdateSubresource(m_MVPConstantBuffer, 0, NULL, &m_MVPStruct, 0, 0);

		{
			using namespace DirectX;
			for (unsigned int i = 0; i < Renderable->GetNumBones(); i++)
			{
				XMMATRIX InverseBindPoseMatrix = XMLoadFloat4x4(&Renderable->GetBindPosePTR()[i]);
				XMMATRIX FrameBoneMatrix = XMLoadFloat4x4(Renderable->GetBoneMatricesPTRPTR()[i]);
				XMStoreFloat4x4(&m_BoneMatrixStruct.Bones[i], (InverseBindPoseMatrix * FrameBoneMatrix));
			}
		}
		m_DeviceContext->UpdateSubresource(m_BoneMatrixConstantBuffer, 0, NULL, &m_BoneMatrixStruct, 0, 0);

		DirectX::XMFLOAT4 CameraPosition(m_CameraWorld->_41, m_CameraWorld->_42, m_CameraWorld->_43, m_CameraWorld->_44);

		m_DeviceContext->UpdateSubresource(m_CameraPosConstantBuffer, 0, NULL, &CameraPosition, 0, 0);

		ID3D11Buffer* Buffers[3]{ m_MVPConstantBuffer, m_BoneMatrixConstantBuffer, m_CameraPosConstantBuffer };

		m_DeviceContext->VSSetConstantBuffers(0, 3, Buffers);

		m_DeviceContext->VSSetShader(Renderable->GetVertexShader(), nullptr, 0);

		m_DeviceContext->PSSetShader(Renderable->GetPixelShader(), nullptr, 0);

		ID3D11ShaderResourceView* SRV[3] = { Renderable->GetTexture(), 	Renderable->GetNormalMap(), Renderable->GetSpecularMap() };

		m_DeviceContext->PSSetShaderResources(0, 3, SRV);

		m_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		m_DeviceContext->DrawIndexed(Renderable->GetIndexCount(), 0, 0);
	}

	void DXManager::shutdown()
	{
		MainAssetManager.shutdown();
		m_RenderTarget->Release();
		m_DepthStencil->Release();
		m_MVPConstantBuffer->Release();
		m_LightingConstantBuffer->Release();
		m_BoneMatrixConstantBuffer->Release();
		m_CameraPosConstantBuffer->Release();
		m_Device->Release();
		m_DeviceContext->Release();
		m_SwapChain->Release();
	}

	void DXManager::SetWindow(HWND _Window)
	{
		m_Window = _Window;
	}

	ID3D11DeviceContext* DXManager::GetDeviceContext()
	{
		return m_DeviceContext;
	}

	ID3D11Device* DXManager::GetDevice()
	{
		return m_Device;
	}

	void DXManager::RegisterVertexShader(std::string Name, ID3D11VertexShader * Shader)
	{
		MainAssetManager.RegisterVertexShader(Name, Shader);
	}

	void DXManager::RegisterInputLayout(std::string Name, ID3D11InputLayout * InputLayout)
	{
		MainAssetManager.RegisterInputLayout(Name, InputLayout);
	}

	void DXManager::RegisterPixelShader(std::string Name, ID3D11PixelShader * Shader)
	{
		MainAssetManager.RegisterPixelShader(Name, Shader);
	}

	void DXManager::RegisterBuffer(std::string Name, ID3D11Buffer * Buffer)
	{
		MainAssetManager.RegisterBuffer(Name, NULL, Buffer);
	}

	void DXManager::RegisterIndexBuffer(std::string Name, unsigned int VertexCount, ID3D11Buffer * Buffer)
	{
		MainAssetManager.RegisterBuffer(Name, VertexCount, Buffer);
	}

	void DXManager::RegisterTexture2D(std::string Name, ID3D11Texture2D * Texture2D)
	{
		MainAssetManager.RegisterTexture2D(Name, Texture2D);
	}

	void DXManager::RegisterShaderResourceView(std::string Name, ID3D11ShaderResourceView * SRV)
	{
		MainAssetManager.RegisterShaderResourceView(Name, SRV);
	}

	void DXManager::CreateRenderable(std::string VertexShader, std::string InputLayout, std::string PixelShader, std::string VertexBuffer, std::string IndexBuffer, std::string Texture, DirectX::XMFLOAT4X4* World, std::vector<EngineAnimation::Bone>* BoneVector, std::string SpecularMap, std::string NormalMap)
	{
		unsigned int Index = (unsigned int)m_RenderList.size();
		m_RenderList.resize(Index + 1);
		m_RenderList[Index].SetVertexShader(VertexShader);
		m_RenderList[Index].SetInputLayout(InputLayout);
		m_RenderList[Index].SetPixelShader(PixelShader);
		m_RenderList[Index].SetVertexBuffer(VertexBuffer);
		m_RenderList[Index].SetTexture(Texture);
		m_RenderList[Index].SetIndexBuffer(IndexBuffer);
		m_RenderList[Index].SetWorldMatrixPTR(World);
		if (BoneVector != nullptr)
		{
			for (unsigned int i = 0; i < (unsigned int)BoneVector->size(); i++)
			{
				m_RenderList[Index].SetBoneMatrixPTR(i, BoneVector->operator[](i).GetWorldMatrixPTR());
				m_RenderList[Index].GetBindPosePTR()[i] = *BoneVector->operator[](i).GetWorldMatrixPTR();
			}
			m_RenderList[Index].SetNumBones((unsigned int)BoneVector->size());
			m_RenderList[Index].RenderMethod = &DXManager::AnimatedRender;
		}
		else
		{
			m_RenderList[Index].RenderMethod = &DXManager::StaticRender;
		}
		if (SpecularMap.size() > 0)
		{
			m_RenderList[Index].RenderMethod = &DXManager::AdvancedAnimatedRender;
			m_RenderList[Index].SetSpecularMap(SpecularMap);
		}
		if (NormalMap.size() > 0)
		{
			m_RenderList[Index].RenderMethod = &DXManager::AdvancedAnimatedRender;
			m_RenderList[Index].SetNormalMap(NormalMap);
		}
	}

	void DXManager::SetViewProjection(DirectX::XMFLOAT4X4 * _CameraView, DirectX::XMFLOAT4X4 * _CameraProjection)
	{
		m_CameraView = _CameraView;
		m_CameraProjection = _CameraProjection;
	}

	void DXManager::SetCameraPosition(DirectX::XMFLOAT4X4 * CameraWorld)
	{
		m_CameraWorld = CameraWorld;
	}

#pragma endregion

	void SetBackBufferDimensions(unsigned int Width, unsigned int Height)
	{
		BackBuffer_Width = Width;
		BackBuffer_Height = Height;
	}

	AssetManager* GetAssetManager() { return &MainAssetManager; }

#pragma region AssetManager

	void AssetManager::CleanVertexShaderMap()
	{
		auto Iter = VertexShaderMap.begin();
		for (; Iter != VertexShaderMap.end(); Iter++)
		{
			Iter->second->Release();
		}
		VertexShaderMap.clear();
	}

	void AssetManager::CleanInputLayoutMap()
	{
		auto Iter = InputLayoutMap.begin();
		for (; Iter != InputLayoutMap.end(); Iter++)
		{
			Iter->second->Release();
		}
		InputLayoutMap.clear();
	}

	void AssetManager::CleanPixelShaderMap()
	{
		auto Iter = PixelShaderMap.begin();
		for (; Iter != PixelShaderMap.end(); Iter++)
		{
			Iter->second->Release();
		}
		PixelShaderMap.clear();
	}

	void AssetManager::CleanBufferMap()
	{
		auto Iter = BufferMap.begin();
		for (; Iter != BufferMap.end(); Iter++)
		{
			Iter->second->Release();
		}
		BufferMap.clear();
	}

	void AssetManager::CleanTexture2DMap()
	{
		auto Iter = Texture2DMap.begin();
		for (; Iter != Texture2DMap.end(); Iter++)
		{
			Iter->second->Release();
		}
		Texture2DMap.clear();
	}

	void AssetManager::CleanShaderResourceViewMap()
	{
		auto Iter = ShaderResourceViewMap.begin();
		for (; Iter != ShaderResourceViewMap.end(); Iter++)
		{
			Iter->second->Release();
		}
		ShaderResourceViewMap.clear();
	}

	void AssetManager::CleanBufferIndexCountMap()
	{
		BufferIndexCountMap.clear();
	}

	void AssetManager::CleanAllMaps()
	{
		CleanVertexShaderMap();
		CleanInputLayoutMap();
		CleanPixelShaderMap();
		CleanBufferMap();
		CleanTexture2DMap();
		CleanShaderResourceViewMap();
		CleanBufferIndexCountMap();
	}

	ID3D11VertexShader * AssetManager::GetVertexShader(std::string Filename)
	{
		if (VertexShaderMap.find(Filename) == VertexShaderMap.end())
		{
			return nullptr;
		}
		return VertexShaderMap[Filename];
	}

	ID3D11InputLayout * AssetManager::GetInputLayout(std::string StructureName)
	{
		if (InputLayoutMap.find(StructureName) == InputLayoutMap.end())
		{
			return nullptr;
		}
		return InputLayoutMap[StructureName];
	}

	ID3D11PixelShader * AssetManager::GetPixelShader(std::string Filename)
	{
		if (PixelShaderMap.find(Filename) == PixelShaderMap.end())
		{
			return nullptr;
		}
		return PixelShaderMap[Filename];
	}

	ID3D11Buffer * AssetManager::GetBuffer(std::string Buffername)
	{
		if (BufferMap.find(Buffername) == BufferMap.end())
		{
			return nullptr;
		}
		return BufferMap[Buffername];
	}

	ID3D11ShaderResourceView * AssetManager::GetShaderResourceView(std::string Filename)
	{
		//		Ensures that a shader resource view is not accessed before it is created.
		if (ShaderResourceViewMap.find(Filename) == ShaderResourceViewMap.end())
		{
			return nullptr;
		}
		return ShaderResourceViewMap[Filename];
	}

	unsigned int AssetManager::GetIndexCount(std::string Buffername)
	{
		if (BufferIndexCountMap.find(Buffername) == BufferIndexCountMap.end())
		{
			return 0;
		}
		return BufferIndexCountMap[Buffername];
	}

	void AssetManager::shutdown()
	{
		CleanAllMaps();
	}

	void AssetManager::RegisterVertexShader(std::string Name, ID3D11VertexShader * Shader)
	{
		//Throws if already exists.
		assert(VertexShaderMap.find(Name) == VertexShaderMap.end());
		VertexShaderMap[Name] = Shader;
	}

	void AssetManager::RegisterInputLayout(std::string Name, ID3D11InputLayout * InputLayout)
	{
		assert(InputLayoutMap.find(Name) == InputLayoutMap.end());
		InputLayoutMap[Name] = InputLayout;
	}

	void AssetManager::RegisterPixelShader(std::string Name, ID3D11PixelShader * Shader)
	{
		assert(PixelShaderMap.find(Name) == PixelShaderMap.end());
		PixelShaderMap[Name] = Shader;
	}

	void AssetManager::RegisterBuffer(std::string Name, unsigned int Indexcount, ID3D11Buffer * Buffer)
	{
		assert(BufferMap.find(Name) == BufferMap.end());
		BufferMap[Name] = Buffer;
		if (Indexcount > 0)
			BufferIndexCountMap[Name] = Indexcount;
	}

	void AssetManager::RegisterTexture2D(std::string Name, ID3D11Texture2D * Texture)
	{
		assert(Texture2DMap.find(Name) == Texture2DMap.end());
		Texture2DMap[Name] = Texture;
	}

	void AssetManager::RegisterShaderResourceView(std::string Name, ID3D11ShaderResourceView * SRV)
	{
		assert(ShaderResourceViewMap.find(Name) == ShaderResourceViewMap.end());
		ShaderResourceViewMap[Name] = SRV;
	}

#pragma endregion
}