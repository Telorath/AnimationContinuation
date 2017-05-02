#pragma once
#include "ExporterHeader.h"
#include <DirectXMath.h>
#include "ObjectStructures.h"
#include <unordered_map>
#include <fbxsdk.h>
#include "../Log.h"

using namespace DirectX;

class FBXLoader
{
	void FBXLoader::InitializeSdkObjects(FbxManager* & pManager, FbxScene* & pScene);
	void DestroySdkObjects(FbxManager* & pManager);
	bool LoadScene(FbxManager* & pManager, FbxScene* & pScene, const char* pFilename);
	bool LoadMesh(FbxNode* node, std::unordered_map<unsigned int, ControlPoint*> mControlPoints, unsigned int& mTriangleCount, std::vector<Triangle>& mTriangles, std::vector<BlendingVertex>& mVertices, unsigned int& vertCount);
	void GetNormals(FbxMesh* inMesh, int inCtrlPointIndex, int inVertexCounter, XMFLOAT3& outNormal);
	void GetUVs(FbxMesh* inMesh, int inCtrlPointIndex, int inVertexCounter, XMFLOAT2& outUV);
	void GetControlPoints(FbxNode* node, std::unordered_map<unsigned int, ControlPoint*>& pControlPoints);
	void GetNode(FbxNode* & final_node,  FbxNodeAttribute::EType attr,FbxNode* node);
	void Optimize(std::vector<BlendingVertex>& mVertices, std::vector<Triangle>& mTriangles);
	int FindVertex(const BlendingVertex& inTargetVertex, const std::vector<BlendingVertex>& uniqueVertices);
	bool LoadBones(FbxNode* node, FbxScene* scene, std::vector<Bone>& bones);
	bool LoadBonesandAnimations(FbxNode* node, FbxScene* scene, Skeleton* & bones, std::vector<Bone> & bind_pose, std::unordered_map<unsigned int, ControlPoint*> & mControlPoints);
	FbxAMatrix GetBindPose(FbxNode* mesh_node, FbxCluster* & cluster);
	void ProcessSkeletonHierarchy(Skeleton* & mSkeleton, FbxNode* inRootNode);
	void ProcessSkeletonHierarchyRecursively(Skeleton* & mSkeleton, FbxNode* inNode, int inDepth, int myIndex, int inParentIndex);
	void ProcessGeometry(std::unordered_map<unsigned int, ControlPoint*> mControlPoints, Skeleton* & mSkeleton, FbxNode* inNode, unsigned int &triCount, std::vector<Triangle> &triIndices, std::vector<BlendingVertex> &verts, bool hasAnimation ,FbxScene* & mScene, std::vector<Bone> & bind_pose, unsigned int & vertCount);
	unsigned int FBXLoader::FindJointIndexUsingName(Skeleton* & mSkeleton, const std::string& inJointName);
	void WriteAnimationToStream(std::ostream& inStream, Skeleton* & mSkeleton);
	XMFLOAT4X4 FBXLoader::FBXMatrixtoXMMatrix(FbxMatrix mat);
	void GetTangent(FbxMesh* inMesh, int inCtrlPointIndex, int inVertexCounter, XMFLOAT3& outTangent);




public:
	bool LoadModel(const char* filename, FileInfo::FILE_TYPES type, unsigned int& triCount, std::vector<Triangle> &triIndices, std::vector<BlendingVertex> &verts, const char* & textureNameOut);
	bool LoadModel(const char* filename, FileInfo::FILE_TYPES type, std::vector<Bone>& bones);
	bool LoadModel(const char* filename, FileInfo::FILE_TYPES type, std::vector<Bone> & bones, unsigned int &triCount, std::vector<Triangle> &triIndices, std::vector<BlendingVertex> &verts, const char* & textureNameOut);
	bool ExportFBX(const char* filename, const char* outFileNameMesh, const char* outFileNameBind, const char* outFileNameAnimations);
	bool ExportMeshFBX(const char * filename, const char * outFileNameMesh);
	FBXLoader();
	~FBXLoader();

};

