#include "ResourceFactory.h"
#include "Rendering.h"
#include "ResourceCreation.h"
#include "SimpleFileIO.h"
#include "DDSTextureLoader.h"
#include "..\FBX\FBXLoadingFunction.h"
#include "Bone.h"
#include "KeyFrame.h"
namespace ResourceFactory
{

	Rendering::DXManager* directXManager;

	void SetDirectXManager(Rendering::DXManager* newDXManager)
	{
		directXManager = newDXManager;
	}

	void CreateBasicPlane()
	{
		ID3D11Buffer* VertexBuffer;
		unsigned int IndexCount;
		ID3D11Buffer* IndexBuffer;
		ResourceCreation::CreatePlane(directXManager->GetDevice(), &VertexBuffer, IndexCount, &IndexBuffer);

		directXManager->RegisterBuffer("PlaneVertexBuffer", VertexBuffer);
		directXManager->RegisterIndexBuffer("PlaneBufferIndices", IndexCount, IndexBuffer);
	}

	void CreateBasicShere()
	{
		ID3D11Buffer* VertexBuffer;
		unsigned int IndexCount;
		ID3D11Buffer* IndexBuffer;
		ResourceCreation::CreateSphere(directXManager->GetDevice(), &VertexBuffer, IndexCount, &IndexBuffer);

		directXManager->RegisterBuffer("SphereVertexBuffer", VertexBuffer);
		directXManager->RegisterIndexBuffer("SphereBufferIndices", IndexCount, IndexBuffer);
	}


	void CreateFBXVertexShader()
	{
		//Creating the first Vertex Shader

		SimpleFileIO VSFile("../x64/Debug/FBXVertexShader.cso");
		VSFile.ReadFileToEnd();
		ID3D11VertexShader* VertexShader;
		ID3D11InputLayout* InputLayout;

		ResourceCreation::CreateVertexShaderPOSUVNORM(directXManager->GetDevice(), *VSFile.GetByteVectorPTR(), &VertexShader, &InputLayout);
		directXManager->RegisterVertexShader("FBXVertexShader", VertexShader);
		directXManager->RegisterInputLayout("ModelViewProjection", InputLayout);
	}

	void CreateAnimationVertexShader()
	{
		//Create the second Vertex Shader
		SimpleFileIO VS2File("../x64/Debug/AnimationVertexShader.cso");
		VS2File.ReadFileToEnd();
		ID3D11VertexShader* AnimationVertexShader;
		ID3D11InputLayout* AnimationInputLayout;

		ResourceCreation::CreateVertexShaderPOSUVNORMBONEWEIGHTBONEINDICES(directXManager->GetDevice(), *VS2File.GetByteVectorPTR(), &AnimationVertexShader, &AnimationInputLayout);
		directXManager->RegisterVertexShader("AnimationVertexShader", AnimationVertexShader);
		directXManager->RegisterInputLayout("AnimationInputLayout", AnimationInputLayout);
	}

	void CreateMappingVertexShader()
	{
		//Create the second Vertex Shader
		SimpleFileIO VSFile("../x64/Debug/AnimationMappingVertexShader.cso");
		VSFile.ReadFileToEnd();
		ID3D11VertexShader* AnimationVertexShader;
		ID3D11InputLayout* AnimationInputLayout;

		ResourceCreation::CreateVertexShaderPOSUVNORMTANBONEWEIGHTBONEINDICES(directXManager->GetDevice(), *VSFile.GetByteVectorPTR(), &AnimationVertexShader, &AnimationInputLayout);
		directXManager->RegisterVertexShader("MappingVertexShader", AnimationVertexShader);
		directXManager->RegisterInputLayout("MappingInputLayout", AnimationInputLayout);
	}

	void CreateUntexturedPixelShader()
	{
		//Create the first pixel shader

		SimpleFileIO PSFile("../x64/Debug/UntexturedLitPixelShader.cso");
		PSFile.ReadFileToEnd();
		ID3D11PixelShader* PixelShader;

		ResourceCreation::CreatePixelShader(directXManager->GetDevice(), *PSFile.GetByteVectorPTR(), &PixelShader);
		directXManager->RegisterPixelShader("UntexturedPixelShader", PixelShader);
	}

	void CreateTexturedPixelShader()
	{
		//create the second pixel shader

		SimpleFileIO PSFile2("../x64/Debug/FBXPixelShader.cso");
		PSFile2.ReadFileToEnd();
		ID3D11PixelShader* PixelShader2;
		ResourceCreation::CreatePixelShader(directXManager->GetDevice(), *PSFile2.GetByteVectorPTR(), &PixelShader2);
		directXManager->RegisterPixelShader("TexturedPixelShader", PixelShader2);
	}

	void CreateMappingPixelShader()
	{
		//create the Third pixel shader

		SimpleFileIO PSFile("../x64/Debug/FBXMappingPixelShader.cso");
		PSFile.ReadFileToEnd();
		ID3D11PixelShader* PixelShader;
		ResourceCreation::CreatePixelShader(directXManager->GetDevice(), *PSFile.GetByteVectorPTR(), &PixelShader);
		directXManager->RegisterPixelShader("MappingPixelShader", PixelShader);
	}

	void CreateBoxTexture()
	{

		//create the box texture

		ID3D11Texture2D* TexturePointer;
		ID3D11ShaderResourceView* SRV;

		CreateDDSTextureFromFile(directXManager->GetDevice(), L"../Box_BindPose.fbm/TestCube.dds", (ID3D11Resource**)&TexturePointer, &SRV);

		directXManager->RegisterTexture2D("CubeTexture", TexturePointer);
		directXManager->RegisterShaderResourceView("CubeTexture", SRV);

	}

	void CreateTeddyTexture()
	{

		//create the box texture

		ID3D11Texture2D* TexturePointer;
		ID3D11ShaderResourceView* SRV;

		CreateDDSTextureFromFile(directXManager->GetDevice(), L"../Teddy_Idle.fbm/Teddy_D.dds", (ID3D11Resource**)&TexturePointer, &SRV);

		directXManager->RegisterTexture2D("TeddyBearTexture", TexturePointer);
		directXManager->RegisterShaderResourceView("TeddyBearTexture", SRV);

	}

	void CreateAnimatedBox(std::vector<EngineAnimation::Bone>* BindPoseOut)
	{
		std::vector<unsigned int> mTriangles;
		std::vector<BlendingVertex> mVerts;

		std::vector<unsigned short> mTriindices;


		unsigned int mTriCount = 0;
		const char* Test = nullptr;


		bool lStatus = ExportFBX("../Box_BindPose.fbx", "../Box_BindPoseMesh.bin", "../Box_BindPoseBind.bin", "../Box_BindPoseAnim.bin");

		Skeleton* Skelly = new Skeleton();
		std::vector<Bone> BindPose;

		lStatus = LoadFBX("../Box_BindPoseMesh.bin", "../Box_BindPoseBind.bin", "../Box_BindPoseAnim.bin", mTriCount, mTriangles, mVerts, Skelly, BindPose);

		for (size_t i = 0; i < BindPose.size(); i++)
		{
			EngineAnimation::Bone B;
			XMStoreFloat4x4(B.GetLocalMatrixPTR(), XMLoadFloat4x4(&BindPose[i].matrix));
//			B.GetLocalMatrixPTR()->_14 *= 0.01f;
//			B.GetLocalMatrixPTR()->_24 *= 1.0f;
//			B.GetLocalMatrixPTR()->_34 *= 1.0f;
			BindPoseOut->push_back(B);
		}

		for (unsigned int i = 0; i < mTriangles.size(); i++)
		{
			mTriindices.push_back(mTriangles[i]);
		}

		ID3D11Buffer* BoxVertexBuffer;
		ID3D11Buffer* BoxIndexBuffer;

		ResourceCreation::DynamicResources::CreateModel(directXManager->GetDevice(), &mVerts[0], (unsigned int)mVerts.size(), &mTriindices[0], (unsigned int)mTriindices.size(), &BoxVertexBuffer, &BoxIndexBuffer);

		directXManager->RegisterBuffer("BoxVertexBuffer", BoxVertexBuffer);
		directXManager->RegisterIndexBuffer("BoxBufferIndices", (unsigned int)mTriindices.size(), BoxIndexBuffer);

		for (size_t i = 0; i < Skelly->joints.size(); i++)
		{
			delete Skelly->joints[i];
		}

		delete Skelly;

	}

	void CreateTeddyBear(std::vector<EngineAnimation::Bone>* BindPoseOut, std::vector<std::vector<EngineAnimation::KeyFrame>>* KeyframesOut)
	{

		KeyframesOut->resize(2);

		std::vector<unsigned int> mTriangles;
		std::vector<BlendingVertex> mVerts;

		std::vector<unsigned short> mTriindices;


		unsigned int mTriCount = 0;
		const char* Test = nullptr;


		bool lStatus = ExportFBX("../Teddy_Idle.fbx", "../Teddy_BindPoseMesh.bin", "../Teddy_BindPoseBind.bin", "../Teddy_BindPoseAnim.bin");


		Skeleton* Skelly = new Skeleton();
		std::vector<Bone> BindPose;

lStatus = LoadFBX("../Teddy_BindPoseMesh.bin", "../Teddy_BindPoseBind.bin", "../Teddy_BindPoseAnim.bin", mTriCount, mTriangles, mVerts, Skelly, BindPose);


		for (size_t i = 0; i < BindPose.size(); i++)
		{
			EngineAnimation::Bone B;
			XMStoreFloat4x4(B.GetLocalMatrixPTR(), XMLoadFloat4x4(&BindPose[i].matrix));
			B.GetLocalMatrixPTR()->_41 *= 0.01f;
			B.GetLocalMatrixPTR()->_42 *= 0.01f;
			B.GetLocalMatrixPTR()->_43 *= 0.01f;
			BindPoseOut->push_back(B);
		}

		KeyframesOut->resize(2);

		for (size_t i = 0; i < Skelly->joints.size(); i++)
		{
			EngineAnimation::KeyFrame NewKeyframe;
			NewKeyframe.SetTime(Skelly->joints[i]->Time);
			for (size_t j = 0; j < Skelly->joints[i]->bones.size(); j++)
			{
				EngineAnimation::Bone B;
				XMStoreFloat4x4(B.GetLocalMatrixPTR(), XMLoadFloat4x4(&Skelly->joints[i]->bones[j].matrix));
				B.GetLocalMatrixPTR()->_41 *= 0.01f;
				B.GetLocalMatrixPTR()->_42 *= 0.01f;
				B.GetLocalMatrixPTR()->_43 *= 0.01f;
				B.GetWorldMatrixPTR();
				XMStoreFloat4x4(B.GetBindMatrixPTR(), XMLoadFloat4x4(&BindPose[j].matrix));
				NewKeyframe.GetBoneVectorPTR()->push_back(B);
			}
			KeyframesOut->operator[](0).push_back(NewKeyframe);
		}

		for (unsigned int i = 0; i < mTriangles.size(); i++)
		{
			mTriindices.push_back(mTriangles[i]);
		}

		ID3D11Buffer* BoxVertexBuffer;
		ID3D11Buffer* BoxIndexBuffer;

	for (size_t i = 0; i < mVerts.size(); i++)
	{
		mVerts[i].mPosition.x *= 0.01f;
		mVerts[i].mPosition.y *= 0.01f;
		mVerts[i].mPosition.z *= 0.01f;
	}

		ResourceCreation::DynamicResources::CreateModel(directXManager->GetDevice(), &mVerts[0], (unsigned int)mVerts.size(), &mTriindices[0], (unsigned int)mTriindices.size(), &BoxVertexBuffer, &BoxIndexBuffer);

		directXManager->RegisterBuffer("TeddyVertexBuffer", BoxVertexBuffer);
		directXManager->RegisterIndexBuffer("TeddyBufferIndices", (unsigned int)mTriindices.size(), BoxIndexBuffer);

		for (size_t i = 0; i < Skelly->joints.size(); i++)
		{
			delete Skelly->joints[i];
		}

		delete Skelly;

		Skelly = new Skeleton();

		lStatus = ExportFBX("../Teddy_Run.fbx", "../Teddy_BindPoseMesh.bin", "../Teddy_BindPoseBind.bin", "../Teddy_RunAnim.bin");
		lStatus = LoadFBX("../Teddy_BindPoseMesh.bin", "../Teddy_BindPoseBind.bin", "../Teddy_RunAnim.bin", mTriCount, mTriangles, mVerts, Skelly, BindPose);


		for (size_t i = 0; i < Skelly->joints.size(); i++)
		{
			EngineAnimation::KeyFrame NewKeyframe;
			NewKeyframe.SetTime(Skelly->joints[i]->Time);
			for (size_t j = 0; j < Skelly->joints[i]->bones.size(); j++)
			{
				EngineAnimation::Bone B;
				XMStoreFloat4x4(B.GetLocalMatrixPTR(), XMLoadFloat4x4(&Skelly->joints[i]->bones[j].matrix));
				B.GetLocalMatrixPTR()->_41 *= 0.01f;
				B.GetLocalMatrixPTR()->_42 *= 0.01f;
				B.GetLocalMatrixPTR()->_43 *= 0.01f;
				B.GetWorldMatrixPTR();
				XMStoreFloat4x4(B.GetBindMatrixPTR(), XMLoadFloat4x4(&BindPose[j].matrix));
				NewKeyframe.GetBoneVectorPTR()->push_back(B);
			}
			KeyframesOut->operator[](1).push_back(NewKeyframe);
		}


		for (size_t i = 0; i < Skelly->joints.size(); i++)
		{
			delete Skelly->joints[i];
		}
		delete Skelly;

	}

	void CreateBoxBones(std::vector<EngineAnimation::Bone>* Bonelist)
	{
		std::vector<Bone> BoneVec;
//		bool lStatus = LoadBones("../Box_BindPose.fbx", FileInfo::FILE_TYPES::BIND_POSE, BoneVec);
		for (size_t i = 0; i < BoneVec.size(); i++)
		{
			EngineAnimation::Bone Newbone;
//			XMVECTOR Rotation = XMLoadFloat4(&BoneVec[i].rotation);
//			XMVECTOR Position = XMLoadFloat4(&BoneVec[i].position);
//			XMStoreFloat4x4(Newbone.GetLocalMatrixPTR(), XMMatrixTranspose(XMMatrixIdentity() * XMMatrixRotationQuaternion(Rotation) * XMMatrixTranslationFromVector(Position)));
			Newbone.MarkDirty();
			Newbone.GetWorldMatrixPTR();
			Bonelist->push_back(Newbone);
		}
	}
	void CreateTeddyBones(std::vector<EngineAnimation::Bone>* Bonelist)
	{
		std::vector<Bone> BoneVec;
//		bool lStatus = LoadBones("../Teddy_Idle.fbx", FileInfo::FILE_TYPES::BIND_POSE, BoneVec);
		for (size_t i = 0; i < BoneVec.size(); i++)
		{
			EngineAnimation::Bone Newbone;
//			XMVECTOR Rotation = XMLoadFloat4(&BoneVec[i].rotation);
//			XMVECTOR Position = XMLoadFloat4(&BoneVec[i].position);
//			XMStoreFloat4x4(Newbone.GetLocalMatrixPTR(), XMMatrixTranspose(XMMatrixIdentity() * XMMatrixRotationQuaternion(Rotation) * XMMatrixTranslationFromVector(Position)));
			Newbone.MarkDirty();
			Newbone.GetWorldMatrixPTR();
			Bonelist->push_back(Newbone);
		}
	}

	void CreateMageTexture()
	{
		//create the box texture

		ID3D11Texture2D* TexturePointer;
		ID3D11ShaderResourceView* SRV;

		CreateDDSTextureFromFile(directXManager->GetDevice(), L"../Mage.fbm/PPG_3D_Player_D.dds", (ID3D11Resource**)&TexturePointer, &SRV);

		directXManager->RegisterTexture2D("MageTexture", TexturePointer);
		directXManager->RegisterShaderResourceView("MageTexture", SRV);


		CreateDDSTextureFromFile(directXManager->GetDevice(), L"../Mage.fbm/PPG_3D_Player_N.dds", (ID3D11Resource**)&TexturePointer, &SRV);

		directXManager->RegisterTexture2D("MageNormalMap", TexturePointer);
		directXManager->RegisterShaderResourceView("MageNormalMap", SRV);

		CreateDDSTextureFromFile(directXManager->GetDevice(), L"../Mage.fbm/PPG_3D_Player_spec.dds", (ID3D11Resource**)&TexturePointer, &SRV);

		directXManager->RegisterTexture2D("MageSpecularMap", TexturePointer);
		directXManager->RegisterShaderResourceView("MageSpecularMap", SRV);

	}

	void CreateMage(std::vector<EngineAnimation::Bone>* BindPoseOut, std::vector<EngineAnimation::KeyFrame>* KeyframesOut)
	{
		std::vector<unsigned int> mTriangles;
		std::vector<BlendingVertex> mVerts;

		std::vector<unsigned int> mTriindices;


		unsigned int mTriCount = 0;
		const char* Test = nullptr;


		bool lStatus = ExportFBX("../Mage_Walk.fbx", "../Mage_BindPoseMesh.bin", "../Mage_BindPoseBind.bin", "../Mage_BindPoseAnim.bin");

		Skeleton* Skelly = new Skeleton();
		std::vector<Bone> BindPose;

		lStatus = LoadFBX("../Mage_BindPoseMesh.bin", "../Mage_BindPoseBind.bin", "../Mage_BindPoseAnim.bin", mTriCount, mTriangles, mVerts, Skelly, BindPose);

		for (size_t i = 0; i < BindPose.size(); i++)
		{
			EngineAnimation::Bone B;
			XMStoreFloat4x4(B.GetLocalMatrixPTR(), XMLoadFloat4x4(&BindPose[i].matrix));
			B.GetLocalMatrixPTR()->_41 *= 0.5f;
			B.GetLocalMatrixPTR()->_42 *= 0.5f;
			B.GetLocalMatrixPTR()->_43 *= 0.5f;
			BindPoseOut->push_back(B);
		}

		for (size_t i = 0; i < Skelly->joints.size(); i++)
		{
			EngineAnimation::KeyFrame NewKeyframe;
			NewKeyframe.SetTime(Skelly->joints[i]->Time);
			for (size_t j = 0; j < Skelly->joints[i]->bones.size(); j++)
			{
				EngineAnimation::Bone B;
				XMStoreFloat4x4(B.GetLocalMatrixPTR(), XMLoadFloat4x4(&Skelly->joints[i]->bones[j].matrix));
				B.GetLocalMatrixPTR()->_41 *= 0.5f;
				B.GetLocalMatrixPTR()->_42 *= 0.5f;
				B.GetLocalMatrixPTR()->_43 *= 0.5f;
				B.GetWorldMatrixPTR();
				XMStoreFloat4x4(B.GetBindMatrixPTR(), XMLoadFloat4x4(&BindPose[j].matrix));
				NewKeyframe.GetBoneVectorPTR()->push_back(B);
			}
			KeyframesOut->push_back(NewKeyframe);
		}

		for (unsigned int i = 0; i < mTriangles.size(); i++)
		{
			mTriindices.push_back(mTriangles[i]);
		}

		ID3D11Buffer* BoxVertexBuffer;
		ID3D11Buffer* BoxIndexBuffer;

		for (size_t i = 0; i < mVerts.size(); i++)
		{
			mVerts[i].mPosition.x *= 0.5f;
			mVerts[i].mPosition.y *= 0.5f;
			mVerts[i].mPosition.z *= 0.5f;
		}

		ResourceCreation::DynamicResources::CreateComplexModel(directXManager->GetDevice(), &mVerts[0], (unsigned int)mVerts.size(), &mTriindices[0], (unsigned int)mTriindices.size(), &BoxVertexBuffer, &BoxIndexBuffer);

		directXManager->RegisterBuffer("MageVertexBuffer", BoxVertexBuffer);
		directXManager->RegisterIndexBuffer("MageBufferIndices", (unsigned int)mTriindices.size(), BoxIndexBuffer);

		for (size_t i = 0; i < Skelly->joints.size(); i++)
		{
			delete Skelly->joints[i];
		}

		delete Skelly;
	}


	void CreateMageBones(std::vector<EngineAnimation::Bone>* Bonelist)
	{
		std::vector<Bone> BoneVec;
		//		bool lStatus = LoadBones("../Teddy_Idle.fbx", FileInfo::FILE_TYPES::BIND_POSE, BoneVec);
		for (size_t i = 0; i < BoneVec.size(); i++)
		{
			EngineAnimation::Bone Newbone;
			//			XMVECTOR Rotation = XMLoadFloat4(&BoneVec[i].rotation);
			//			XMVECTOR Position = XMLoadFloat4(&BoneVec[i].position);
			//			XMStoreFloat4x4(Newbone.GetLocalMatrixPTR(), XMMatrixTranspose(XMMatrixIdentity() * XMMatrixRotationQuaternion(Rotation) * XMMatrixTranslationFromVector(Position)));
			Newbone.MarkDirty();
			Newbone.GetWorldMatrixPTR();
			Bonelist->push_back(Newbone);
		}
	}

	bool CreateObjectDataFromFBX(std::vector<EngineAnimation::Bone>* BindPoseOut, std::vector<std::vector<EngineAnimation::KeyFrame>>* KeyframesOut, std::vector<std::string> FBXPaths, std::vector<std::string> AnimationNames, std::string ObjectName)
	{
		
		if (FBXPaths.size() != AnimationNames.size())
		{
			return false;
		}

		KeyframesOut->resize(FBXPaths.size());

		std::vector<unsigned int> mTriangles;
		std::vector<BlendingVertex> mVerts;
		std::vector<unsigned short> mTriindices;


		unsigned int mTriCount = 0;
		const char* Test = nullptr;

		//Get Bind Pose

		std::string FBX = FBXPaths[0];
		std::string BindPoseMesh = "../" + ObjectName + "_BindPoseMesh.bin";
		std::string BindPoseBind = "../" + ObjectName + "_BindPoseBind.bin";
		std::string BindPoseAnim = "../" + ObjectName + "_BindPoseAnim.bin";

		bool lStatus = ExportFBX(FBX.c_str(), BindPoseMesh.c_str(), BindPoseBind.c_str(), BindPoseAnim.c_str());

		Skeleton* Skelly = new Skeleton();
		std::vector<Bone> BindPose;

		lStatus = LoadFBX("../Teddy_BindPoseMesh.bin", "../Teddy_BindPoseBind.bin", "../Teddy_BindPoseAnim.bin", mTriCount, mTriangles, mVerts, Skelly, BindPose);


		for (size_t i = 0; i < BindPose.size(); i++)
		{
			EngineAnimation::Bone B;
			XMStoreFloat4x4(B.GetLocalMatrixPTR(), XMLoadFloat4x4(&BindPose[i].matrix));
			B.GetLocalMatrixPTR()->_41 *= 0.01f;
			B.GetLocalMatrixPTR()->_42 *= 0.01f;
			B.GetLocalMatrixPTR()->_43 *= 0.01f;
			BindPoseOut->push_back(B);
		}

		for (size_t i = 0; i < Skelly->joints.size(); i++)
		{
			EngineAnimation::KeyFrame NewKeyframe;
			NewKeyframe.SetTime(Skelly->joints[i]->Time);
			for (size_t j = 0; j < Skelly->joints[i]->bones.size(); j++)
			{
				EngineAnimation::Bone B;
				XMStoreFloat4x4(B.GetLocalMatrixPTR(), XMLoadFloat4x4(&Skelly->joints[i]->bones[j].matrix));
//				B.GetLocalMatrixPTR()->_41 *= 0.01f;
//				B.GetLocalMatrixPTR()->_42 *= 0.01f;
//				B.GetLocalMatrixPTR()->_43 *= 0.01f;
				B.GetWorldMatrixPTR();
				XMStoreFloat4x4(B.GetBindMatrixPTR(), XMLoadFloat4x4(&BindPose[j].matrix));
				NewKeyframe.GetBoneVectorPTR()->push_back(B);
			}
			KeyframesOut->operator[](0).push_back(NewKeyframe);
		}

		for (unsigned int i = 0; i < mTriangles.size(); i++)
		{
			mTriindices.push_back(mTriangles[i]);
		}

		ID3D11Buffer* BoxVertexBuffer;
		ID3D11Buffer* BoxIndexBuffer;

//		for (size_t i = 0; i < mVerts.size(); i++)
//		{
//			mVerts[i].mPosition.x *= 0.01f;
//			mVerts[i].mPosition.y *= 0.01f;
//			mVerts[i].mPosition.z *= 0.01f;
//		}

		ResourceCreation::DynamicResources::CreateModel(directXManager->GetDevice(), &mVerts[0], (unsigned int)mVerts.size(), &mTriindices[0], (unsigned int)mTriindices.size(), &BoxVertexBuffer, &BoxIndexBuffer);

		directXManager->RegisterBuffer("TeddyVertexBuffer", BoxVertexBuffer);
		directXManager->RegisterIndexBuffer("TeddyBufferIndices", (unsigned int)mTriindices.size(), BoxIndexBuffer);

		for (size_t i = 0; i < Skelly->joints.size(); i++)
		{
			delete Skelly->joints[i];
		}

		delete Skelly;

		Skelly = new Skeleton();

		lStatus = ExportFBX("../Teddy_Run.fbx", "../Teddy_BindPoseMesh.bin", "../Teddy_BindPoseBind.bin", "../Teddy_RunAnim.bin");
		lStatus = LoadFBX("../Teddy_BindPoseMesh.bin", "../Teddy_BindPoseBind.bin", "../Teddy_RunAnim.bin", mTriCount, mTriangles, mVerts, Skelly, BindPose);


		for (size_t i = 0; i < Skelly->joints.size(); i++)
		{
			EngineAnimation::KeyFrame NewKeyframe;
			NewKeyframe.SetTime(Skelly->joints[i]->Time);
			for (size_t j = 0; j < Skelly->joints[i]->bones.size(); j++)
			{
				EngineAnimation::Bone B;
				XMStoreFloat4x4(B.GetLocalMatrixPTR(), XMLoadFloat4x4(&Skelly->joints[i]->bones[j].matrix));
				B.GetLocalMatrixPTR()->_41 *= 0.01f;
				B.GetLocalMatrixPTR()->_42 *= 0.01f;
				B.GetLocalMatrixPTR()->_43 *= 0.01f;
				B.GetWorldMatrixPTR();
				XMStoreFloat4x4(B.GetBindMatrixPTR(), XMLoadFloat4x4(&BindPose[j].matrix));
				NewKeyframe.GetBoneVectorPTR()->push_back(B);
			}
			KeyframesOut->operator[](1).push_back(NewKeyframe);
		}


		for (size_t i = 0; i < Skelly->joints.size(); i++)
		{
			delete Skelly->joints[i];
		}
		delete Skelly;

	}
}