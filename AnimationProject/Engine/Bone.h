#pragma once
#include <DirectXMath.h>
namespace EngineAnimation
{

class Bone
{
	//Since every animation is a modification of this bind pose matrix, we should probably keep a record of it and never ever ever change it.
	DirectX::XMFLOAT4X4 m_BindPoseMatrix;
	DirectX::XMFLOAT4X4 m_Local;
	DirectX::XMFLOAT4X4 m_World;
	bool Dirty;
	void Clean();
public:
	Bone();
	~Bone();
	void SetLocalMatrix(const DirectX::XMFLOAT4X4& NewMatrix);
	void SetBindPoseMatrix(const DirectX::XMFLOAT4X4 NewMatrix);
	void MarkDirty();
	DirectX::XMFLOAT4X4* GetBindMatrixPTR();
	DirectX::XMFLOAT4X4* GetLocalMatrixPTR();
	DirectX::XMFLOAT4X4* GetWorldMatrixPTR();
};

}