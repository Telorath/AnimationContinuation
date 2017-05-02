#include "FBXLoadingFunction.h"
#include "FBXLoader.h"

//bool LoadMesh(char* filename, FileInfo::FILE_TYPES type, unsigned int &triCount, std::vector<Triangle> &triIndices, std::vector<BlendingVertex> &verts, const char* & texName)
//{
//	FBXLoader loader;
//	return loader.LoadModel(filename, type, triCount, triIndices, verts, texName);
//}
//
//bool LoadBones(char* filename, FileInfo::FILE_TYPES type, std::vector<Bone> &bones)
//{
//	FBXLoader loader;
//	return loader.LoadModel(filename, type, bones);
//}

DLLEXPORT bool ExportFBX(const char* filename, const char * outFilenameMesh, const char* outFileNameBind, const char* outFileNameAnimations)
{
	FBXLoader loader;
	return loader.ExportFBX(filename, outFilenameMesh, outFileNameBind, outFileNameAnimations);
}

DLLEXPORT bool LoadMeshFBX(const char* filename, unsigned int& triCount, std::vector<unsigned int>&triIndices, std::vector<BlendingVertex> &verts)
{
	FILE* file;

	bool bReturn = false;

	file = nullptr;

	fopen_s(&file, filename, "rb");
	if (file)
	{
		bReturn = true;
		triCount = 0;
		unsigned int vertCount = 0;
		fread(&triCount, sizeof(unsigned int), 1, file);
		fread(&vertCount, sizeof(unsigned int), 1, file);
		triIndices.resize(triCount * 3);
		fread(&triIndices[0], sizeof(unsigned int), triCount * 3, file);

		verts.resize(vertCount);
		for (unsigned int i = 0; i < vertCount; ++i)
		{
			unsigned int blendInfoSize = 0;
			BlendingVertex temp;
			fread(&temp.mPosition, sizeof(float), 3, file);
			fread(&temp.mNormal, sizeof(float), 3, file);
			fread(&temp.mUV, sizeof(float), 2, file);
			fread(&temp.mTangent, sizeof(float), 3, file);
			fread(&blendInfoSize, sizeof(unsigned int), 1, file);
			for (unsigned int j = 0; j < blendInfoSize; ++j)
			{
				VertexBlending tempBlend;
				fread(&tempBlend.mBlendIndex, sizeof(unsigned int), 1, file);
				fread(&tempBlend.mBlendWeight, sizeof(double), 1, file);
				temp.mVertexBlendInfos.push_back(tempBlend);
			}
			verts[i] = temp;
		}
		fclose(file);
		return true;
	}
	return false;
}

DLLEXPORT bool LoadBonesFBX(const char* filenameBone, std::vector<Bone> &bind_pose)
{
	FILE* file;
	file = nullptr;
	fopen_s(&file, filenameBone, "rb");
	if (file)
	{
		unsigned int bindCount = 0;
		fread(&bindCount, sizeof(unsigned int), 1, file);
		bind_pose.resize(bindCount);
		fread(&bind_pose[0], sizeof(Bone), bindCount, file);
		fclose(file);
		return true;
	}
	return false;
}

DLLEXPORT bool LoadAnimationFBX(const char* fileNameAnimations, Skeleton* & mSkeleton)
{
	FILE* file;
	file = nullptr;
	fopen_s(&file, fileNameAnimations, "rb");
	if (file)
	{
		unsigned int frameCount = 0;

		fread(&frameCount, sizeof(unsigned int), 1, file);


		mSkeleton->joints.resize(frameCount);
		for (unsigned int i = 0; i < frameCount; ++i)
		{
			KeyFrame* temp = new KeyFrame();
			unsigned int bone_size = 0;
			fread(&bone_size, sizeof(unsigned int), 1, file);
			temp->bones.resize(bone_size);
			fread(&temp->bones[0], sizeof(Bone), bone_size, file);
			fread(&temp->Time, sizeof(float), 1, file);

			mSkeleton->joints[i] = temp;
		}
		fclose(file);
 	return true;
	}
	return false;
}

DLLEXPORT bool LoadFBX(const char* filename, const char* filenameBone, const char* fileNameAnimations, unsigned int& triCount, std::vector<unsigned int>&triIndices, std::vector<BlendingVertex> &verts, Skeleton* & mSkeleton, std::vector<Bone> &bind_pose)
{
	bool bReturn = false;

	bReturn = LoadMeshFBX(filename, triCount, triIndices, verts);

	bReturn = LoadBonesFBX(filenameBone, bind_pose);

	bReturn = LoadAnimationFBX(fileNameAnimations, mSkeleton);

	return bReturn;
}

