#include "ResourceCreation.h"
#include "Rendering.h"
#include <DirectXMath.h>
#include "..\FBX\ObjectStructures.h"
#include <cmath>

#define PI 3.141592654f

using namespace DirectX;

struct VertexPositionUVNormal
{
	XMFLOAT3 Position;
	XMFLOAT3 UV;
	XMFLOAT3 Normal;
};

struct VertexPositionUVBlendWeightBoneIndices
{
	XMFLOAT3 Position;
	XMFLOAT3 UV;
	XMFLOAT3 Normal;
	XMFLOAT4 BoneWeights;
	XMUINT4 BoneIndices;
};

namespace ResourceCreation
{
	void CreateVertexShaderPOSUVNORM(ID3D11Device* Device, const std::vector<char>& FileData, ID3D11VertexShader** VertShader, ID3D11InputLayout** InputLayout)
	{
		Device->CreateVertexShader(&FileData[0], FileData.size(), nullptr, VertShader);

		static const D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "UV", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 },
		};
		Device->CreateInputLayout(vertexDesc, ARRAYSIZE(vertexDesc), &FileData[0], FileData.size(), InputLayout);
	}
	void CreateVertexShaderPOSUVNORMBONEWEIGHTBONEINDICES(ID3D11Device* Device, const std::vector<char>& FileData, ID3D11VertexShader** VertShader, ID3D11InputLayout** InputLayout)
	{
		Device->CreateVertexShader(&FileData[0], FileData.size(), nullptr, VertShader);

		static const D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "UV", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 },
			{ "BLENDWEIGHTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,0},
			{ "BLENDINDICES", 0, DXGI_FORMAT_R32G32B32A32_UINT,0,D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,0 },
		};
		Device->CreateInputLayout(vertexDesc, ARRAYSIZE(vertexDesc), &FileData[0], FileData.size(), InputLayout);
	}

	void CreateVertexShaderPOSUVNORMTANBONEWEIGHTBONEINDICES(ID3D11Device* Device, const std::vector<char>& FileData, ID3D11VertexShader** VertShader, ID3D11InputLayout** InputLayout)
	{
		Device->CreateVertexShader(&FileData[0], FileData.size(), nullptr, VertShader);

		static const D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "UV", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 },
			{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 },
			{ "BLENDWEIGHTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,0 },
			{ "BLENDINDICES", 0, DXGI_FORMAT_R32G32B32A32_UINT,0,D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,0 },
		};
		Device->CreateInputLayout(vertexDesc, ARRAYSIZE(vertexDesc), &FileData[0], FileData.size(), InputLayout);
	}

	void CreateSphere(ID3D11Device* Device, ID3D11Buffer** VertexBuffer, unsigned int& IndexCount, ID3D11Buffer** IndexBuffer)
	{
		//Mathematically create a sphere.
		int stackCount = 20;
		int sliceCount = 20;
		float radius = 0.10f;
		std::vector<VertexPositionUVNormal> VertexVector;
		VertexVector.reserve(20);
		VertexVector.push_back(VertexPositionUVNormal{ {0.0f, radius, 0.0f}, { 0, 0, 0 }, { 0, 1, 0 } });
		float phiStep = PI / stackCount;
		float thetaStep = 2.0f * PI / sliceCount;

		for (int i = 1; i <= stackCount - 1; i++) {
			float phi = i*phiStep;
			for (int j = 0; j <= sliceCount; j++) {
				float theta = j*thetaStep;
				XMFLOAT3 p = XMFLOAT3(
					(radius * sinf(phi)*cosf(theta)),
					(radius * cosf(phi)),
					(radius * sinf(phi)*sinf(theta))
				);

				XMFLOAT3 t = XMFLOAT3(-radius*sinf(phi)*sinf(theta), 0, radius*sinf(phi)*cosf(theta));
				XMStoreFloat3(&t, XMVector3Normalize(XMLoadFloat3(&t)));
				XMFLOAT3 n;
				XMStoreFloat3(&n, XMVector3Normalize(XMLoadFloat3(&p)));
				XMFLOAT3 uv = XMFLOAT3({ theta / (PI * 2), phi / PI, 0 });
				VertexVector.push_back(VertexPositionUVNormal{ { p.x, p.y, p.z }, { uv.x, uv.y, uv.z }, { n.x, n.y, n.z } });
			}
		}
		VertexVector.push_back(VertexPositionUVNormal{ {0, -radius, 0}, { 0, 1,0 }, {0, -1, 0} });

		//Get our list of indices

		std::vector<unsigned short> Indices;
		Indices.reserve(20);

		for (int i = 1; i <= sliceCount; i++) {
			Indices.push_back(0);
			Indices.push_back(i + 1);
			Indices.push_back(i);
		}
		unsigned short baseIndex = 1;
		unsigned short ringVertexCount = sliceCount + 1;
		for (int i = 0; i < stackCount - 2; i++) {
			for (int j = 0; j < sliceCount; j++) {
				Indices.push_back(baseIndex + i*ringVertexCount + j);
				Indices.push_back(baseIndex + i*ringVertexCount + j + 1);
				Indices.push_back(baseIndex + (i + 1)*ringVertexCount + j);

				Indices.push_back(baseIndex + (i + 1)*ringVertexCount + j);
				Indices.push_back(baseIndex + i*ringVertexCount + j + 1);
				Indices.push_back(baseIndex + (i + 1)*ringVertexCount + j + 1);
			}
		}
		unsigned short southPoleIndex = (unsigned short)VertexVector.size() - 1;
		baseIndex = southPoleIndex - ringVertexCount;
		for (int i = 0; i < sliceCount; i++) {
			Indices.push_back(southPoleIndex);
			Indices.push_back(baseIndex + i);
			Indices.push_back(baseIndex + i + 1);
		}


		//Dear god. Alright now we can finally create our buffers.
		IndexCount = (unsigned int)Indices.size();

		D3D11_SUBRESOURCE_DATA vertexBufferData = { 0 };
		vertexBufferData.pSysMem = &VertexVector[0];
		vertexBufferData.SysMemPitch = 0;
		vertexBufferData.SysMemSlicePitch = 0;
		CD3D11_BUFFER_DESC vertexBufferDesc(sizeof(VertexPositionUVNormal) * (UINT)VertexVector.size(), D3D11_BIND_VERTEX_BUFFER);
		Device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, VertexBuffer);


		D3D11_SUBRESOURCE_DATA indexBufferData = { 0 };
		indexBufferData.pSysMem = &Indices[0];
		indexBufferData.SysMemPitch = 0;
		indexBufferData.SysMemSlicePitch = 0;
		CD3D11_BUFFER_DESC indexBufferDesc(sizeof(unsigned short) * (UINT)Indices.size(), D3D11_BIND_INDEX_BUFFER);
		Device->CreateBuffer(&indexBufferDesc, &indexBufferData, IndexBuffer);

	}

	void CreatePixelShader(ID3D11Device* Device, const std::vector<char>& FileData, ID3D11PixelShader** PixelShader)
	{
		Device->CreatePixelShader(&FileData[0], FileData.size(), nullptr, PixelShader);
	}
	void CreatePlane(ID3D11Device* Device, ID3D11Buffer** VertexBuffer, unsigned int& IndexCount, ID3D11Buffer** IndexBuffer/*, ModelConstantBuffer& Transform*/)
	{
		// Load mesh vertices. Each vertex has a position and a color.
		static const VertexPositionUVNormal PlaneVertices[] =
		{
			{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f) },
			{ XMFLOAT3(-1.0f, -1.0f,  1.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f) },
			{ XMFLOAT3(1.0f,  -1.0f, -1.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f) },
			{ XMFLOAT3(1.0f,  -1.0f,  1.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f) },

			{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f) },
			{ XMFLOAT3(-1.0f, -1.0f,  1.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f) },
			{ XMFLOAT3(1.0f,  -1.0f, -1.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f) },
			{ XMFLOAT3(1.0f,  -1.0f,  1.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f) },

		};
		//		XMStoreFloat4x4(&Transform.model, XMMatrixTranspose(XMMatrixRotationY(0)));
		D3D11_SUBRESOURCE_DATA vertexBufferData = { 0 };
		vertexBufferData.pSysMem = PlaneVertices;
		vertexBufferData.SysMemPitch = 0;
		vertexBufferData.SysMemSlicePitch = 0;
		CD3D11_BUFFER_DESC vertexBufferDesc(sizeof(PlaneVertices), D3D11_BIND_VERTEX_BUFFER);
		Device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, VertexBuffer);

		// Load mesh indices. Each trio of indices represents
		// a triangle to be rendered on the screen.
		// For example: 0,2,1 means that the vertices with indexes
		// 0, 2 and 1 from the vertex buffer compose the 
		// first triangle of this mesh.
		static const unsigned short PlaneIndices[] =
		{
			0,1,2,
			1,3,2,

			6,5,4,
			6,7,5,
		};

		IndexCount = ARRAYSIZE(PlaneIndices);

		D3D11_SUBRESOURCE_DATA indexBufferData = { 0 };
		indexBufferData.pSysMem = PlaneIndices;
		indexBufferData.SysMemPitch = 0;
		indexBufferData.SysMemSlicePitch = 0;
		CD3D11_BUFFER_DESC indexBufferDesc(sizeof(PlaneIndices), D3D11_BIND_INDEX_BUFFER);
		Device->CreateBuffer(&indexBufferDesc, &indexBufferData, IndexBuffer);
	}

	void CreateAnimatedPlane(ID3D11Device* Device, ID3D11Buffer** VertexBuffer, unsigned int& IndexCount, ID3D11Buffer** IndexBuffer/*, ModelConstantBuffer& Transform*/)
	{
		// Load mesh vertices. Each vertex has a position and a color.
		static const VertexPositionUVBlendWeightBoneIndices PlaneVertices[] =
		{
			{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(1,0,0,0), XMUINT4(0,0,0,0) },
			{ XMFLOAT3(-1.0f, -1.0f,  1.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(1,0,0,0), XMUINT4(0,0,0,0) },
			{ XMFLOAT3(1.0f,  -1.0f, -1.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(1,0,0,0), XMUINT4(0,0,0,0) },
			{ XMFLOAT3(1.0f,  -1.0f,  1.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(1,0,0,0), XMUINT4(0,0,0,0) },

			{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT4(1,0,0,0), XMUINT4(0,0,0,0) },
			{ XMFLOAT3(-1.0f, -1.0f,  1.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT4(1,0,0,0), XMUINT4(0,0,0,0) },
			{ XMFLOAT3(1.0f,  -1.0f, -1.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT4(1,0,0,0), XMUINT4(0,0,0,0) },
			{ XMFLOAT3(1.0f,  -1.0f,  1.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT4(1,0,0,0), XMUINT4(0,0,0,0) },

		};
		//		XMStoreFloat4x4(&Transform.model, XMMatrixTranspose(XMMatrixRotationY(0)));
		D3D11_SUBRESOURCE_DATA vertexBufferData = { 0 };
		vertexBufferData.pSysMem = PlaneVertices;
		vertexBufferData.SysMemPitch = 0;
		vertexBufferData.SysMemSlicePitch = 0;
		CD3D11_BUFFER_DESC vertexBufferDesc(sizeof(PlaneVertices), D3D11_BIND_VERTEX_BUFFER);
		Device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, VertexBuffer);

		// Load mesh indices. Each trio of indices represents
		// a triangle to be rendered on the screen.
		// For example: 0,2,1 means that the vertices with indexes
		// 0, 2 and 1 from the vertex buffer compose the 
		// first triangle of this mesh.
		static const unsigned short PlaneIndices[] =
		{
			0,1,2,
			1,3,2,

			6,5,4,
			6,7,5,
		};

		IndexCount = ARRAYSIZE(PlaneIndices);

		D3D11_SUBRESOURCE_DATA indexBufferData = { 0 };
		indexBufferData.pSysMem = PlaneIndices;
		indexBufferData.SysMemPitch = 0;
		indexBufferData.SysMemSlicePitch = 0;
		CD3D11_BUFFER_DESC indexBufferDesc(sizeof(PlaneIndices), D3D11_BIND_INDEX_BUFFER);
		Device->CreateBuffer(&indexBufferDesc, &indexBufferData, IndexBuffer);
	}
	namespace DynamicResources
	{
		void CreateModel(ID3D11Device* Device, BlendingVertex* BlendingVerticesData, unsigned int numverts, unsigned short* TriangleIndices, unsigned int IndexCount, ID3D11Buffer** VertexBuffer, ID3D11Buffer** IndexBuffer)
		{
#if true

			VertexPositionUVBlendWeightBoneIndices* Vertices = new VertexPositionUVBlendWeightBoneIndices[numverts];

			for (size_t i = 0; i < numverts; i++)
			{
				Vertices[i].Position = BlendingVerticesData[i].mPosition;
				Vertices[i].UV.x = BlendingVerticesData[i].mUV.x;
				Vertices[i].UV.y = BlendingVerticesData[i].mUV.y;
				Vertices[i].Normal = BlendingVerticesData[i].mNormal;
				unsigned int numblends = (unsigned int)BlendingVerticesData[i].mVertexBlendInfos.size();
				if (BlendingVerticesData[i].mVertexBlendInfos.size() > 0)
				{
					Vertices[i].BoneWeights.x = (float)BlendingVerticesData[i].mVertexBlendInfos[0].mBlendWeight;
					Vertices[i].BoneWeights.y = (float)BlendingVerticesData[i].mVertexBlendInfos[1].mBlendWeight;
					Vertices[i].BoneWeights.z = (float)BlendingVerticesData[i].mVertexBlendInfos[2].mBlendWeight;
					Vertices[i].BoneWeights.w = (float)BlendingVerticesData[i].mVertexBlendInfos[3].mBlendWeight;
					Vertices[i].BoneIndices.x = BlendingVerticesData[i].mVertexBlendInfos[0].mBlendIndex;
					Vertices[i].BoneIndices.y = BlendingVerticesData[i].mVertexBlendInfos[1].mBlendIndex;
					Vertices[i].BoneIndices.z = BlendingVerticesData[i].mVertexBlendInfos[2].mBlendIndex;
					Vertices[i].BoneIndices.w = BlendingVerticesData[i].mVertexBlendInfos[3].mBlendIndex;
				}
				else
				{
					Vertices[i].BoneWeights.x = 1.0f;
					Vertices[i].BoneWeights.y = 0.0f;
					Vertices[i].BoneWeights.z = 0.0f;
					Vertices[i].BoneWeights.w = 0.0f;

					Vertices[i].BoneIndices.x = 3;
					Vertices[i].BoneIndices.y = 3;
					Vertices[i].BoneIndices.z = 3;
					Vertices[i].BoneIndices.w = 3;

				}

			}
			D3D11_SUBRESOURCE_DATA vertexBufferData = { 0 };
			vertexBufferData.pSysMem = Vertices;
			vertexBufferData.SysMemPitch = 0;
			vertexBufferData.SysMemSlicePitch = 0;
			CD3D11_BUFFER_DESC vertexBufferDesc(sizeof(VertexPositionUVBlendWeightBoneIndices) * numverts, D3D11_BIND_VERTEX_BUFFER);
			Device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, VertexBuffer);

			delete[] Vertices;

			D3D11_SUBRESOURCE_DATA indexBufferData = { 0 };
			indexBufferData.pSysMem = TriangleIndices;
			indexBufferData.SysMemPitch = 0;
			indexBufferData.SysMemSlicePitch = 0;
			CD3D11_BUFFER_DESC indexBufferDesc(sizeof(unsigned short) * IndexCount, D3D11_BIND_INDEX_BUFFER);
			Device->CreateBuffer(&indexBufferDesc, &indexBufferData, IndexBuffer);
#endif
		}

		void CreateComplexModel(ID3D11Device* Device, BlendingVertex* BlendingVerticesData, unsigned int numverts, unsigned int* TriangleIndices, unsigned int IndexCount, ID3D11Buffer** VertexBuffer, ID3D11Buffer** IndexBuffer)
		{
#if true
			
			Rendering::VertexPositionUVNormalTangentBlendWeightBoneIndices* Vertices = new Rendering::VertexPositionUVNormalTangentBlendWeightBoneIndices[numverts];

			for (size_t i = 0; i < numverts; i++)
			{
				Vertices[i].Position = BlendingVerticesData[i].mPosition;
				Vertices[i].UV.x = BlendingVerticesData[i].mUV.x;
				Vertices[i].UV.y = BlendingVerticesData[i].mUV.y;
				Vertices[i].Normal = BlendingVerticesData[i].mNormal;
				Vertices[i].Tangent = BlendingVerticesData[i].mTangent;
				unsigned int numblends = (unsigned int)BlendingVerticesData[i].mVertexBlendInfos.size();
				if (BlendingVerticesData[i].mVertexBlendInfos.size() > 0)
				{
					Vertices[i].BoneWeights.x = (float)BlendingVerticesData[i].mVertexBlendInfos[0].mBlendWeight;
					Vertices[i].BoneWeights.y = (float)BlendingVerticesData[i].mVertexBlendInfos[1].mBlendWeight;
					Vertices[i].BoneWeights.z = (float)BlendingVerticesData[i].mVertexBlendInfos[2].mBlendWeight;
					Vertices[i].BoneWeights.w = (float)BlendingVerticesData[i].mVertexBlendInfos[3].mBlendWeight;
					Vertices[i].BoneIndices.x = BlendingVerticesData[i].mVertexBlendInfos[0].mBlendIndex;
					Vertices[i].BoneIndices.y = BlendingVerticesData[i].mVertexBlendInfos[1].mBlendIndex;
					Vertices[i].BoneIndices.z = BlendingVerticesData[i].mVertexBlendInfos[2].mBlendIndex;
					Vertices[i].BoneIndices.w = BlendingVerticesData[i].mVertexBlendInfos[3].mBlendIndex;
				}
				else
				{
					Vertices[i].BoneWeights.x = 1.0f;
					Vertices[i].BoneWeights.y = 0.0f;
					Vertices[i].BoneWeights.z = 0.0f;
					Vertices[i].BoneWeights.w = 0.0f;

					Vertices[i].BoneIndices.x = 3;
					Vertices[i].BoneIndices.y = 3;
					Vertices[i].BoneIndices.z = 3;
					Vertices[i].BoneIndices.w = 3;

				}

			}
			D3D11_SUBRESOURCE_DATA vertexBufferData = { 0 };
			vertexBufferData.pSysMem = Vertices;
			vertexBufferData.SysMemPitch = 0;
			vertexBufferData.SysMemSlicePitch = 0;
			CD3D11_BUFFER_DESC vertexBufferDesc(sizeof(Rendering::VertexPositionUVNormalTangentBlendWeightBoneIndices) * numverts, D3D11_BIND_VERTEX_BUFFER);
			Device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, VertexBuffer);

			delete[] Vertices;

			D3D11_SUBRESOURCE_DATA indexBufferData = { 0 };
			indexBufferData.pSysMem = TriangleIndices;
			indexBufferData.SysMemPitch = 0;
			indexBufferData.SysMemSlicePitch = 0;
			CD3D11_BUFFER_DESC indexBufferDesc(sizeof(unsigned int) * IndexCount, D3D11_BIND_INDEX_BUFFER);
			Device->CreateBuffer(&indexBufferDesc, &indexBufferData, IndexBuffer);
#endif
		}

	}
}
