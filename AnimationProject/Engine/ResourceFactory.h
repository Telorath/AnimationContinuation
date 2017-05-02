#pragma once
#include <vector>

namespace Rendering {
	class DXManager;
}
namespace EngineAnimation
{
	class Bone;
	class KeyFrame;
}

namespace ResourceFactory
{

	void SetDirectXManager(Rendering::DXManager* newDXManager);

	void CreateBasicPlane();

	void CreateBasicShere();

	void CreateFBXVertexShader();

	void CreateAnimationVertexShader();

	void CreateMappingVertexShader();

	void CreateUntexturedPixelShader();

	void CreateTexturedPixelShader();

	void CreateMappingPixelShader();

	void CreateBoxTexture();

	void CreateAnimatedBox(std::vector<EngineAnimation::Bone>* BindPoseOut);

	void CreateTeddyTexture();

	void CreateTeddyBear(std::vector<EngineAnimation::Bone>* BindPoseOut, std::vector<std::vector<EngineAnimation::KeyFrame>>* KeyframesOut);

	void CreateBoxBones(std::vector<EngineAnimation::Bone>* Bonelist);

	void CreateTeddyBones(std::vector<EngineAnimation::Bone>* Bonelist);

	void CreateMageTexture();

	void CreateMage(std::vector<EngineAnimation::Bone>* BindPoseOut, std::vector<EngineAnimation::KeyFrame>* KeyframesOut);

	void CreateMageBones(std::vector<EngineAnimation::Bone>* Bonelist);

};

