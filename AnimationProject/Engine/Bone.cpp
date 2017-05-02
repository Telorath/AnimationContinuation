#include "Bone.h"

using namespace DirectX;

namespace EngineAnimation
{
	//Experimental
	void Bone::Clean()
	{
		m_World = m_Local;
	}
	Bone::Bone()
	{
	}


	Bone::~Bone()
	{
	}

	void Bone::SetLocalMatrix(const DirectX::XMFLOAT4X4 & NewMatrix)
	{
		m_Local = NewMatrix;
		m_World = m_Local;
	}

	void Bone::SetBindPoseMatrix(const DirectX::XMFLOAT4X4 NewMatrix)
	{
		m_BindPoseMatrix = NewMatrix;
	}

	void Bone::MarkDirty()
	{
		Dirty = true;
	}

	DirectX::XMFLOAT4X4 * Bone::GetBindMatrixPTR()
	{
		return &m_BindPoseMatrix;
	}

	DirectX::XMFLOAT4X4 * Bone::GetLocalMatrixPTR()
	{
		return &m_Local;
	}

	DirectX::XMFLOAT4X4 * Bone::GetWorldMatrixPTR()
	{
		if (Dirty)
		{
			Clean();
		}
		return &m_World;
	}
}
