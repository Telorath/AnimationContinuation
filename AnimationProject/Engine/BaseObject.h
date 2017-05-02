#pragma once
#include <DirectXMath.h>
#include <vector>
#include "Interpolator.h"

namespace EngineAnimation 
{
	class Bone;
	class KeyFrame;
}

class BaseObject
{
protected:
	DirectX::XMFLOAT4X4 m_World;
	std::vector<EngineAnimation::Bone> Bones;
	float time;
	//-1 is bind pose.
	int KeyFrameIndex = -1;
	std::vector<std::vector<EngineAnimation::KeyFrame>> Animations;
	unsigned int ActiveAnimation = 0;
	Interpolator m_Interpolator;
public:
	BaseObject();
	~BaseObject();
	virtual void init();
	virtual void update(float DeltaTime);
	virtual void shutdown();
	DirectX::XMFLOAT4X4* GetWorldMatrixPTR();
	std::vector<EngineAnimation::KeyFrame>* GetKeyFrameList();
	std::vector<std::vector<EngineAnimation::KeyFrame>>* GetAnimationList();
	std::vector<EngineAnimation::Bone>* GetBonesList();
};