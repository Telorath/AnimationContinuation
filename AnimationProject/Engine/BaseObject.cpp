#include "BaseObject.h"
#include "Bone.h"
#include "KeyFrame.h"
#include "InputManager.h"

using namespace EngineAnimation;

BaseObject::BaseObject()
{
}


BaseObject::~BaseObject()
{
}

void BaseObject::init()
{
	m_Interpolator.SetBoneVectorPTR(&Bones);
	if (Animations.size() > 0)
	{
		m_Interpolator.SetKeyFramePTR(&Animations[0]);
		for (size_t i = 0; i < Animations.size(); i++)
		{
			if (Animations[i][0].GetTime() == 0.0f)
			{
				Animations[i].erase(Animations[i].begin());
			}
		}
	}
}

void BaseObject::update(float DeltaTime)
{
	//This key is to switch frames.
	if (Animations.size() > 0)
	{
		if (InputManager::getkeydown(InputManager::Button::keypad_equals))
		{
			//LET'S TRY BLENDING!
			EngineAnimation::KeyFrame BlendingFrame;
			BlendingFrame.SetTime(-0.1f);
			for (size_t i = 0; i < Bones.size(); i++)
			{
				Bone B;
				B.SetLocalMatrix(*Bones[i].GetWorldMatrixPTR());
				BlendingFrame.GetBoneVectorPTR()->push_back(B);
			}
			ActiveAnimation++;
			if (ActiveAnimation >= Animations.size())
			{
				ActiveAnimation = 0;
			}
			m_Interpolator.SetKeyFramePTR(&Animations[ActiveAnimation]);
			m_Interpolator.Blend(BlendingFrame);
		}
		m_Interpolator.Update(DeltaTime);
	}
}

void BaseObject::shutdown()
{
}

DirectX::XMFLOAT4X4 * BaseObject::GetWorldMatrixPTR()
{
	return &m_World;
}

std::vector<EngineAnimation::KeyFrame>* BaseObject::GetKeyFrameList()
{
	if (Animations.size() < 1)
	{
		Animations.resize(1);
		ActiveAnimation = 0;
	}
	return &Animations[ActiveAnimation];
}

std::vector<std::vector<EngineAnimation::KeyFrame>>* BaseObject::GetAnimationList()
{
	return &Animations;
}

std::vector<Bone>* BaseObject::GetBonesList()
{
	return &Bones;
}
