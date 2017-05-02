#pragma once
#include <vector>
#include "KeyFrame.h"
namespace EngineAnimation
{
	class KeyFrame;
	class Bone;
}
class Interpolator
{
	std::vector<EngineAnimation::Bone>* Bones;
	std::vector<EngineAnimation::KeyFrame>* KeyFrames;
	EngineAnimation::KeyFrame BlendingFrame;
	int PreviousIndex = 0;
	//We're technically more often than not somewhere between these two.
	int NextIndex = 1;
	float time = 0;
public:
	Interpolator();
	~Interpolator();
	void SetBoneVectorPTR(std::vector<EngineAnimation::Bone>* NewBones);
	void SetKeyFramePTR(std::vector<EngineAnimation::KeyFrame>* NewKeyFrames);
	void Update(float DeltaTime);
	void Reset(float DeltaTime);
	void Blend(EngineAnimation::KeyFrame newBlendingFrame);
};

