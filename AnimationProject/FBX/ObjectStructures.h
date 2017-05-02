#pragma once
#include <DirectXMath.h>
#include <vector>
#include <algorithm>
using namespace DirectX;

//epsilon vectors for comparing close to 0
const XMFLOAT2 vector2Epsilon = XMFLOAT2(0.00001f, 0.00001f);
const XMFLOAT3 vector3Epsilon = XMFLOAT3(0.00001f, 0.00001f, 0.00001f);
struct RegularVertex
{
	XMFLOAT3 mPosition;
	XMFLOAT3 mNormal;
	XMFLOAT2 mUV;

	bool operator==(const RegularVertex& rhs) const
	{
		uint32_t position;
		uint32_t normal;
		uint32_t uv;

		XMVectorEqualR(&position, XMLoadFloat3(&(this->mPosition)), XMLoadFloat3(&rhs.mPosition));
		XMVectorEqualR(&normal, XMLoadFloat3(&(this->mNormal)), XMLoadFloat3(&rhs.mNormal));
		XMVectorEqualR(&uv, XMLoadFloat2(&(this->mUV)), XMLoadFloat2(&rhs.mUV));

		return XMComparisonAllTrue(position) && XMComparisonAllTrue(normal) && XMComparisonAllTrue(uv);
	}
};

struct VertexBlending
{
	unsigned int mBlendIndex;
	double mBlendWeight;

	VertexBlending() :
		mBlendIndex(0),
		mBlendWeight(0.0)
	{}

	bool operator < (const VertexBlending& rhs)
	{
		return (mBlendWeight > rhs.mBlendWeight);
	}
};

struct BlendingVertex
{
	XMFLOAT3 mPosition;
	XMFLOAT3 mNormal;
	XMFLOAT2 mUV;
	XMFLOAT3 mTangent;
	std::vector<VertexBlending> mVertexBlendInfos;


	void SortBlendingInfoByWeight()
	{
		std::sort(mVertexBlendInfos.begin(), mVertexBlendInfos.end());
	}

	bool operator==(const BlendingVertex& rhs) const
	{
		bool sameBlendingInfo = true;

		// We only compare the blending info when there is blending info
		if (!(mVertexBlendInfos.empty() && rhs.mVertexBlendInfos.empty()))
		{
			// Each vertex should only have 4 index-weight blending info pairs
			for (unsigned int i = 0; i < 4; ++i)
			{
				if (mVertexBlendInfos[i].mBlendIndex != rhs.mVertexBlendInfos[i].mBlendIndex ||
					abs(mVertexBlendInfos[i].mBlendWeight - rhs.mVertexBlendInfos[i].mBlendWeight) > 0.001)
				{
					sameBlendingInfo = false;
					break;
				}
			}
		}

		bool result1 = XMVector3NearEqual(XMLoadFloat3(&mPosition), XMLoadFloat3(&rhs.mPosition), XMLoadFloat3(&vector3Epsilon));
		bool result2 = XMVector3NearEqual(XMLoadFloat3(&mNormal), XMLoadFloat3(&rhs.mNormal), XMLoadFloat3(&vector3Epsilon));
		bool result3 = XMVector3NearEqual(XMLoadFloat2(&mUV), XMLoadFloat2(&rhs.mUV), XMLoadFloat2(&vector2Epsilon));

		return result1 && result2 && result3 && sameBlendingInfo;
	}
};

struct BlendIndWeightPair
{
	unsigned int mBlendIndex;
	double mBlendWeight;

	BlendIndWeightPair() :
		mBlendIndex(0),
		mBlendWeight(0)
	{}
};

struct ControlPoint
{
	XMFLOAT3 mPosition;
	std::vector<BlendIndWeightPair> mBlendInfo;

	ControlPoint()
	{
		mBlendInfo.reserve(4);
	}
};

struct Triangle
{
	std::vector<unsigned int> mIndices;
};

struct Bone
{
	XMFLOAT4X4 matrix;
};

struct KeyFrame
{
	float Time;
	std::vector<Bone> bones;
};

struct Skeleton
{
	std::vector<KeyFrame*> joints;
};
