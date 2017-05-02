#pragma once
#include <DirectXMath.h>
#include "Bone.h"
#include <vector>
namespace EngineAnimation
{
	class KeyFrame
	{
		float time;
		std::vector<EngineAnimation::Bone> Bonevector;
	public:
		KeyFrame();
		~KeyFrame();

		std::vector<EngineAnimation::Bone>* GetBoneVectorPTR();
		float GetTime();
		void SetTime(float newtime);
	};
}

