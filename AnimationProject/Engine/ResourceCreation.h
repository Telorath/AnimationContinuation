#pragma once
#include <string>
#include <d3d11.h>
#include <vector>

struct BlendingVertex;
struct Triangle;

namespace ResourceCreation
{
	namespace StaticResources 
	{

	};
	void CreateVertexShaderPOSUVNORM(ID3D11Device* Device, const std::vector<char>& FileData, ID3D11VertexShader** VertShader, ID3D11InputLayout** InputLayout);
	void CreateVertexShaderPOSUVNORMBONEWEIGHTBONEINDICES(ID3D11Device* Device, const std::vector<char>& FileData, ID3D11VertexShader** VertShader, ID3D11InputLayout** InputLayout);
	void CreateVertexShaderPOSUVNORMTANBONEWEIGHTBONEINDICES(ID3D11Device* Device, const std::vector<char>& FileData, ID3D11VertexShader** VertShader, ID3D11InputLayout** InputLayout);
	void CreatePixelShader(ID3D11Device* Device, const std::vector<char>& FileData, ID3D11PixelShader** PixelShader);
	void CreateSphere(ID3D11Device* Device, ID3D11Buffer** VertexBuffer, unsigned int& IndexCount, ID3D11Buffer** IndexBuffer);
	void CreatePlane(ID3D11Device* Device, ID3D11Buffer** VertexBuffer, unsigned int& IndexCount, ID3D11Buffer** IndexBuffer);
	void CreateAnimatedPlane(ID3D11Device* Device, ID3D11Buffer** VertexBuffer, unsigned int& IndexCount, ID3D11Buffer** IndexBuffer);
	namespace DynamicResources
	{
		void CreateModel(ID3D11Device* Device, BlendingVertex* BlendingVerticesData, unsigned int numverts, unsigned short* TriangleIndices, unsigned int IndexCount, ID3D11Buffer** VertexBuffer, ID3D11Buffer** IndexBuffer);
		void CreateComplexModel(ID3D11Device* Device, BlendingVertex* BlendingVerticesData, unsigned int numverts, unsigned int* TriangleIndices, unsigned int IndexCount, ID3D11Buffer** VertexBuffer, ID3D11Buffer** IndexBuffer);
	}
};