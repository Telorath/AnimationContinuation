#include "Interpolator.h"
#include "KeyFrame.h"
#include "Bone.h"

void SLERPMATRIX(DirectX::XMFLOAT4X4* FinalMatrix, DirectX::XMFLOAT4X4* InitialMatrix, float t, DirectX::XMFLOAT4X4* Outptr)
{
	using namespace DirectX;

	XMVECTOR InitialRotation;
	XMVECTOR InitialScale;
	XMVECTOR InitialPosition;

	XMVECTOR FinalRotation;
	XMVECTOR FinalScale;
	XMVECTOR FinalPosition;

	XMVECTOR NowRotation;
	XMVECTOR NowScale;
	XMVECTOR NowPosition;

	XMMatrixDecompose(&InitialScale, &InitialRotation, &InitialPosition, XMLoadFloat4x4(InitialMatrix));
	XMMatrixDecompose(&FinalScale, &FinalRotation, &FinalPosition, XMLoadFloat4x4(FinalMatrix));

	NowRotation = XMQuaternionSlerp(InitialRotation, FinalRotation, t);
	NowScale = XMVectorLerp(InitialScale, FinalScale, t);
	NowPosition = XMVectorLerp(InitialPosition, FinalPosition, t);

	XMStoreFloat4x4(Outptr, XMMatrixAffineTransformation(NowScale, XMVectorZero(), NowRotation, NowPosition));
}

Interpolator::Interpolator()
{
}


Interpolator::~Interpolator()
{
}

void Interpolator::SetBoneVectorPTR(std::vector<EngineAnimation::Bone>* NewBones)
{
	Bones = NewBones;
}

void Interpolator::SetKeyFramePTR(std::vector<EngineAnimation::KeyFrame>* NewKeyFrames)
{
	KeyFrames = NewKeyFrames;
}

void Interpolator::Update(float DeltaTime)
{
	time += DeltaTime;
	while (time >= KeyFrames->operator[](NextIndex).GetTime())
	{
		PreviousIndex++;
		NextIndex++;
		if (NextIndex >= KeyFrames->size())
		{
			NextIndex = 0;
		}
		if (PreviousIndex >= KeyFrames->size())
		{
			PreviousIndex = 0;
		}
		if (time > KeyFrames->operator[](KeyFrames->size() - 1).GetTime())
		{
			time -= KeyFrames->operator[](KeyFrames->size() - 1).GetTime();
		}
	}
	EngineAnimation::KeyFrame* PreviousFrame;
	if (PreviousIndex == -1)
	{
		PreviousFrame = &BlendingFrame;
	}
	else
	{
		PreviousFrame = &KeyFrames->operator[](PreviousIndex);
	}
	EngineAnimation::KeyFrame* NextFrame = &KeyFrames->operator[](NextIndex);

	float lasttime = PreviousFrame->GetTime();
	if (PreviousIndex == KeyFrames->size() - 1)
	{
		lasttime = 0.0f;
	}
	float nexttime = NextFrame->GetTime();
	float t = (time - lasttime) / (nexttime - lasttime);

	for (size_t i = 0; i < Bones->size(); i++)
	{
		DirectX::XMFLOAT4X4* WorldMatrix = Bones->operator[](i).GetLocalMatrixPTR();
		DirectX::XMFLOAT4X4* InitialMatrix = PreviousFrame->GetBoneVectorPTR()->operator[](i).GetWorldMatrixPTR();
		DirectX::XMFLOAT4X4* FinalMatrix = NextFrame->GetBoneVectorPTR()->operator[](i).GetWorldMatrixPTR();

		SLERPMATRIX(FinalMatrix, InitialMatrix, t, WorldMatrix);

		Bones->operator[](i).GetWorldMatrixPTR();

	}
}

void Interpolator::Reset(float DeltaTime)
{
	PreviousIndex = 0;
	NextIndex = 1;
	time = 0;
}

void Interpolator::Blend(EngineAnimation::KeyFrame newBlendingFrame)
{
	BlendingFrame = newBlendingFrame;
	PreviousIndex = -1;
	NextIndex = 0;
	time = BlendingFrame.GetTime();
}
