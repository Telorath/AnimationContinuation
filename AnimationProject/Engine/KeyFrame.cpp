#include "KeyFrame.h"

namespace EngineAnimation
{
	KeyFrame::KeyFrame()
	{
	}


	KeyFrame::~KeyFrame()
	{
	}

	std::vector<EngineAnimation::Bone>* KeyFrame::GetBoneVectorPTR()
	{
		return &Bonevector;
	}

	float KeyFrame::GetTime()
	{
		return time;
	}

	void KeyFrame::SetTime(float newtime)
	{
		time = newtime;
	}

}