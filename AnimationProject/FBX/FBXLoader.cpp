#include "FBXLoader.h"



FBXLoader::FBXLoader()
{
}


FBXLoader::~FBXLoader()
{
}

//bool FBXLoader::LoadModel(char* filename, FileInfo::FILE_TYPES type, unsigned int &triCount, std::vector<Triangle> &triIndices, std::vector<BlendingVertex> &verts, const char* & textureNameOut)
//{
//	FbxManager* mManager = nullptr;
//	FbxScene* mScene = nullptr;
//
//	bool bReturn = false;
//
//	InitializeSdkObjects(mManager, mScene);
//	if (mManager)
//	{
//		bReturn = LoadScene(mManager, mScene, filename);
//		FbxGeometryConverter* converter = new FbxGeometryConverter(mManager);
//		converter->Triangulate(mScene, true);
//		delete converter;
//		switch (type)
//		{
//		case FileInfo::FILE_TYPES::MESH:
//		{
//			std::unordered_map<unsigned int, ControlPoint*> mControlPoints;
//			FbxNode* mNode = nullptr;
//			GetNode(mNode, FbxNodeAttribute::eMesh, mScene->GetRootNode());
//			GetControlPoints(mNode, mControlPoints);
//			LoadMesh(mNode, mControlPoints, triCount, triIndices, verts, textureNameOut);
//			break;
//		}
//		case FileInfo::FILE_TYPES::BIND_POSE:
//		{
//			return false;
//			break;
//		}
//		default:
//			break;
//		}
//	}
//	DestroySdkObjects(mManager);
//	return bReturn;
//}

//bool FBXLoader::LoadModel(char* filename, FileInfo::FILE_TYPES type, std::vector<Bone>& bones)
//{
//	FbxManager* mManager = nullptr;
//	FbxScene* mScene = nullptr;
//
//	bool bReturn = false;
//
//	InitializeSdkObjects(mManager, mScene);
//	if (mManager)
//	{
//		bReturn = LoadScene(mManager, mScene, filename);
//		FbxGeometryConverter* converter = new FbxGeometryConverter(mManager);
//		converter->Triangulate(mScene, true);
//		delete converter;
//		switch (type)
//		{
//		case FileInfo::FILE_TYPES::MESH:
//		{
//			return false;
//			break;
//		}
//		case FileInfo::FILE_TYPES::BIND_POSE:
//		{
//			std::unordered_map<unsigned int, ControlPoint*> mControlPoints;
//			FbxNode* mNode = nullptr;
//			GetNode(mNode, FbxNodeAttribute::eMesh, mScene->GetRootNode());
//			GetControlPoints(mNode, mControlPoints);
//			LoadBones(mNode, mScene, bones);
//			break;
//		}
//		default:
//			break;
//		}
//	}
//	DestroySdkObjects(mManager);
//	return bReturn;
//}

//bool FBXLoader::LoadModel(char*filename, FileInfo::FILE_TYPES type, std::vector<Bone> & bones, unsigned int &triCount, std::vector<Triangle> &triIndices, std::vector<BlendingVertex> &verts, const char* & textureNameOut)
//{
//	
//	FbxManager* mManager = nullptr;
//	FbxScene* mScene = nullptr;
//
//	bool bReturn = false;
//
//	InitializeSdkObjects(mManager, mScene);
//	if (mManager)
//	{
//		bReturn = LoadScene(mManager, mScene, filename);
//		FbxGeometryConverter* converter = new FbxGeometryConverter(mManager);
//		converter->Triangulate(mScene, true);
//		delete converter;
//		switch (type)
//		{
//		case FileInfo::FILE_TYPES::MESH:
//		{
//			return false;
//			break;
//		}
//		case FileInfo::FILE_TYPES::BIND_POSE:
//		{
//			return false;
//			break;
//		}
//		case FileInfo::FILE_TYPES::ANIMATION:
//		{
//			std::unordered_map<unsigned int, ControlPoint*> mControlPoints;
//			FbxNode* mNode = nullptr;
//			GetNode(mNode, FbxNodeAttribute::eMesh, mScene->GetRootNode());
//			GetControlPoints(mNode, mControlPoints);
//			LoadMesh(mNode, mControlPoints, triCount, triIndices, verts, textureNameOut);
//			LoadBonesandAnimations(mNode, mScene, bones);
//			break;
//		}
//		default:
//			break;
//		}
//	}
//	DestroySdkObjects(mManager);
//	return bReturn;
//}

void FBXLoader::InitializeSdkObjects(FbxManager* & pManager, FbxScene* & pScene)
{
	//Create a manager
	pManager = FbxManager::Create();

	//Create an IOS setting
	FbxIOSettings* ios = FbxIOSettings::Create(pManager, IOSROOT);
	pManager->SetIOSettings(ios);

	FbxString lPath = FbxGetApplicationDirectory();
	pManager->LoadPluginsDirectory(lPath.Buffer());

	//Create an FBX scene.
	pScene = FbxScene::Create(pManager, "MyScene");
}

void FBXLoader::DestroySdkObjects(FbxManager* & pManager)
{
	//Destroy the pManager and all linked objects
	if (pManager) pManager->Destroy();
}

bool FBXLoader::LoadScene(FbxManager* & pManager, FbxScene* & pScene, const char* pFilename)
{
	bool lStatus;

	// Create an importer.
	FbxImporter* lImporter = FbxImporter::Create(pManager, "");

	// Initialize the importer by providing a filename.
	const bool lImportStatus = lImporter->Initialize(pFilename, -1, pManager->GetIOSettings());


	// Import the scene.
	lStatus = lImporter->Import(pScene);

	// Destroy the importer.
	lImporter->Destroy();

	return lStatus;
}

bool FBXLoader::LoadMesh(FbxNode* node, std::unordered_map<unsigned int, ControlPoint*> mControlPoints, unsigned int& mTriangleCount, std::vector<Triangle>& mTriangles, std::vector<BlendingVertex>& mVertices, unsigned int& vertCount)
{
	FbxMesh* currMesh = node->GetMesh();
	FbxVector4* see = currMesh->GetControlPoints();
	mTriangleCount = currMesh->GetPolygonCount();
	int vertexCounter = 0;
	mTriangles.reserve(mTriangleCount);

	for (unsigned int i = 0; i < mTriangleCount; ++i)
	{
		XMFLOAT3 normal[3];
		XMFLOAT2 UV[3];
		XMFLOAT3 tangent[3];
		Triangle currTriangle;
		mTriangles.push_back(currTriangle);
		for (unsigned int j = 0; j < 3; ++j)
		{
			int ctrlPointIndex = currMesh->GetPolygonVertex(i, j);
			ControlPoint* currCtrlPoint = mControlPoints[ctrlPointIndex];


			GetNormals(currMesh, ctrlPointIndex, vertexCounter, normal[j]);
			//normal[j].z *= -1;
			int k = 0;
			GetUVs(currMesh, ctrlPointIndex, vertexCounter, UV[j]);
			//UV[j][k].y = 1.0f - UV[j][k].y;
			GetTangent(currMesh, ctrlPointIndex, vertexCounter, tangent[j]);
		//currCtrlPoint->mPosition.z *= -1;


			BlendingVertex temp;
			temp.mPosition = currCtrlPoint->mPosition;
			temp.mPosition.z *= -1;
			temp.mNormal = normal[j];
			temp.mNormal.z *= -1;
			temp.mUV = UV[j];
			temp.mUV.y = 1.0f - temp.mUV.y;
			temp.mTangent = tangent[j];
			temp.mTangent.z *= -1;

			// Copy the blending info from each control point
			for (unsigned int i = 0; i < currCtrlPoint->mBlendInfo.size(); ++i)
			{
				VertexBlending currBlendingInfo;
				currBlendingInfo.mBlendIndex = currCtrlPoint->mBlendInfo[i].mBlendIndex;
				currBlendingInfo.mBlendWeight = currCtrlPoint->mBlendInfo[i].mBlendWeight;
				temp.mVertexBlendInfos.push_back(currBlendingInfo);
			}
			// Sort the blending info so that later we can remove
			// duplicated vertices
			temp.SortBlendingInfoByWeight();
			mVertices.push_back(temp);
			mTriangles.back().mIndices.push_back(vertexCounter);
			++vertexCounter;
		}

		vertCount = vertexCounter;
	}

#if 0
	int materialCount = node->GetSrcObjectCount<FbxSurfaceMaterial>();

	for (int index = 0; index < materialCount; index++)
	{
		FbxSurfaceMaterial* material = (FbxSurfaceMaterial*)node->GetSrcObject<FbxSurfaceMaterial>(index);

		if (material != NULL)
		{
			// This only gets the material of type sDiffuse, you probably need to traverse all Standard Material Property by its name to get all possible textures.
			FbxProperty prop = material->FindProperty(FbxSurfaceMaterial::sDiffuse);

			// Check if it's layeredtextures
			int layeredTextureCount = prop.GetSrcObjectCount<FbxLayeredTexture>();

			if (layeredTextureCount > 0)
			{
				for (int j = 0; j < layeredTextureCount; j++)
				{
					FbxLayeredTexture* layered_texture = FbxCast<FbxLayeredTexture>(prop.GetSrcObject<FbxLayeredTexture>(j));
					int lcount = layered_texture->GetSrcObjectCount<FbxTexture>();

					for (int k = 0; k < lcount; k++)
					{
						FbxTexture* texture = FbxCast<FbxTexture>(layered_texture->GetSrcObject<FbxTexture>(k));
						// Then, you can get all the properties of the texture, include its name
						const char* textureName = FbxCast<FbxFileTexture>(texture)->GetFileName();
						textureNameOut = textureName;

					}
				}
			}
			else
			{
				// Directly get textures
				int textureCount = prop.GetSrcObjectCount<FbxTexture>();
				for (int j = 0; j < textureCount; j++)
				{
					FbxTexture* texture = FbxCast<FbxTexture>(prop.GetSrcObject<FbxTexture>(j));
					// Then, you can get all the properties of the texture, include its name
					const char* textureName = FbxCast<FbxFileTexture>(texture)->GetFileName();

					textureNameOut = textureName;
					//FbxProperty p = texture->RootProperty.Find("Filename");
				}
			}
		}
	}

#endif
	//Optimize(mVertices, mTriangles);

	//Free Control Points
	for (auto itr = mControlPoints.begin(); itr != mControlPoints.end(); ++itr)
	{
		delete itr->second;
	}
	mControlPoints.clear();
	return true;
}

//bool FBXLoader::LoadBones(FbxNode* node, FbxScene* scene, std::vector<Bone>& bones)
//{
//	FbxMesh* mesh = node->GetMesh();
//	unsigned int numDeformers = mesh->GetDeformerCount();
//
//	for (unsigned int deformerIndex = 0; deformerIndex < numDeformers; ++deformerIndex)
//	{
//		FbxSkin* skin = reinterpret_cast<FbxSkin*>(mesh->GetDeformer(deformerIndex, FbxDeformer::eSkin));
//		if (!skin)
//			continue;
//		unsigned int numClusters = skin->GetClusterCount();
//		for (unsigned int clusterIndex = 0; clusterIndex < numClusters; ++clusterIndex)
//		{
//			FbxCluster* cluster = skin->GetCluster(clusterIndex);
//			if (!cluster)
//				continue;
//			FbxAMatrix global_bone = GetBindPose(node, cluster);
//			
//			FbxVector4 rot = global_bone.GetR(), pos = global_bone.GetT();
//			rot.Set(-rot.mData[0], -rot.mData[1], rot.mData[2]);
//			pos.Set(pos.mData[0], pos.mData[1], -pos.mData[2]);
//			XMFLOAT4 bone_rot = { (float)rot[0], (float)rot[1], (float)rot[2], (float)rot[3] }, bone_pos = { (float)pos[0], (float)pos[1], -(float)pos[2], (float)pos[3] };
//			Bone newbone = { bone_pos, bone_rot };
//			bones.push_back(newbone);
//		}
//	}
//	return true;
//}

bool FBXLoader::LoadBonesandAnimations(FbxNode* node, FbxScene* scene, Skeleton* & bones, std::vector<Bone> & bind_pose, std::unordered_map<unsigned int, ControlPoint*> & mControlPoints)
{
	FbxMesh* mesh = node->GetMesh();
	unsigned int numDeformers = mesh->GetDeformerCount();

	for (unsigned int deformerIndex = 0; deformerIndex < numDeformers; ++deformerIndex)
	{
		FbxSkin* skin = reinterpret_cast<FbxSkin*>(mesh->GetDeformer(deformerIndex, FbxDeformer::eSkin));
		if (!skin)
			continue;
		FbxAnimStack* currAnimStack = scene->GetSrcObject<FbxAnimStack>(0);
		FbxString animStackName = currAnimStack->GetName();
		std::string animationName = animStackName.Buffer();
		FbxTakeInfo* takeInfo = scene->GetTakeInfo(animStackName);
		FbxTime start = takeInfo->mLocalTimeSpan.GetStart();
		FbxTime end = takeInfo->mLocalTimeSpan.GetStop();
		FbxLongLong mAnimationLength = end.GetFrameCount(FbxTime::eFrames24) - start.GetFrameCount(FbxTime::eFrames24) + 1;
		for (FbxLongLong i = start.GetFrameCount(FbxTime::eFrames24); i <= end.GetFrameCount(FbxTime::eFrames24); ++i)
		{
			FbxTime currTime;
			currTime.SetFrame(i, FbxTime::eFrames24);
			KeyFrame* currAnim = new KeyFrame();
			currAnim->Time = (float)currTime.GetSecondDouble();
			FbxAMatrix currentTransformOffset = node->EvaluateGlobalTransform(currTime);
			
			unsigned int numClusters = skin->GetClusterCount();
			for (unsigned int clusterIndex = 0; clusterIndex < numClusters; ++clusterIndex)
			{
				FbxCluster* cluster = skin->GetCluster(clusterIndex);
				if (!cluster)
					continue;

				FbxAMatrix tranMat;
				FbxAMatrix tranMatLink;
				cluster->GetTransformMatrix(tranMat);
				cluster->GetTransformLinkMatrix(tranMatLink);


				FbxAMatrix global_bone = /*GetBindPose(node, cluster);*/ tranMatLink.Inverse() * tranMat;

				if (start.GetFrameCount(FbxTime::eFrames24) == i)
				{
					Bone newbone;
					newbone.matrix = FBXMatrixtoXMMatrix(global_bone);
					bind_pose.push_back(newbone);
					unsigned int numIndices = cluster->GetControlPointIndicesCount();
					for (unsigned int j = 0; j < numIndices; ++j)
					{
						int ctrlPointINdex = cluster->GetControlPointIndices()[j];
						BlendIndWeightPair currBlend;
						currBlend.mBlendIndex = clusterIndex;
						currBlend.mBlendWeight = cluster->GetControlPointWeights()[j];
						mControlPoints[ctrlPointINdex]->mBlendInfo.push_back(currBlend);
					}
				}

				FbxMatrix bone_mat = node->EvaluateGlobalTransform(currTime);
				XMFLOAT4X4 bone;
				bone = FBXMatrixtoXMMatrix(bone_mat.Inverse()*cluster->GetLink()->EvaluateGlobalTransform(currTime));
				Bone b; b.matrix = bone;
				currAnim->bones.push_back(b);
			}
			bones->joints.push_back(currAnim);
		}
	}

	BlendIndWeightPair currBlendingIndexWeightPair;
	currBlendingIndexWeightPair.mBlendIndex = 0;
	currBlendingIndexWeightPair.mBlendWeight = 0;
	for (auto itr = mControlPoints.begin(); itr != mControlPoints.end(); ++itr)
	{
		for (unsigned int i = (unsigned int)itr->second->mBlendInfo.size(); i <= 4; ++i)
		{
			itr->second->mBlendInfo.push_back(currBlendingIndexWeightPair);
		}
	}
	return true;
}

void FBXLoader::GetNormals(FbxMesh* inMesh, int inCtrlPointIndex, int inVertexCounter, XMFLOAT3& outNormal)
{
	if (inMesh->GetElementNormalCount() < 1)
	{
		throw std::exception("Invalid Normal Number");
	}

	FbxGeometryElementNormal* vertexNormal = inMesh->GetElementNormal(0);
	switch (vertexNormal->GetMappingMode())
	{
	case FbxGeometryElement::eByControlPoint:
		switch (vertexNormal->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			outNormal.x = static_cast<float>(vertexNormal->GetDirectArray().GetAt(inCtrlPointIndex).mData[0]);
			outNormal.y = static_cast<float>(vertexNormal->GetDirectArray().GetAt(inCtrlPointIndex).mData[1]);
			outNormal.z = static_cast<float>(vertexNormal->GetDirectArray().GetAt(inCtrlPointIndex).mData[2]);
		}
		break;

		case FbxGeometryElement::eIndexToDirect:
		{
			int index = vertexNormal->GetIndexArray().GetAt(inCtrlPointIndex);
			outNormal.x = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[0]);
			outNormal.y = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[1]);
			outNormal.z = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[2]);
		}
		break;

		default:
			throw std::exception("Invalid Reference");
		}
		break;

	case FbxGeometryElement::eByPolygonVertex:
		switch (vertexNormal->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			outNormal.x = static_cast<float>(vertexNormal->GetDirectArray().GetAt(inVertexCounter).mData[0]);
			outNormal.y = static_cast<float>(vertexNormal->GetDirectArray().GetAt(inVertexCounter).mData[1]);
			outNormal.z = static_cast<float>(vertexNormal->GetDirectArray().GetAt(inVertexCounter).mData[2]);
		}
		break;

		case FbxGeometryElement::eIndexToDirect:
		{
			int index = vertexNormal->GetIndexArray().GetAt(inVertexCounter);
			outNormal.x = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[0]);
			outNormal.y = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[1]);
			outNormal.z = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[2]);
		}
		break;

		default:
			throw std::exception("Invalid Reference");
		}
		break;
	}
}

void FBXLoader::GetUVs(FbxMesh* inMesh, int inCtrlPointIndex, int inVertexCounter, XMFLOAT2& outUV)
{
	if (inMesh->GetElementUVCount() < 1)
	{
		throw std::exception("Invalid UV Number");
	}

	FbxGeometryElementUV* vertexUV = inMesh->GetElementUV(0);
	switch (vertexUV->GetMappingMode())
	{
	case FbxGeometryElement::eByControlPoint:
		switch (vertexUV->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			outUV.x = static_cast<float>(vertexUV->GetDirectArray().GetAt(inCtrlPointIndex).mData[0]);
			outUV.y = static_cast<float>(vertexUV->GetDirectArray().GetAt(inCtrlPointIndex).mData[1]);
		}
		break;

		case FbxGeometryElement::eIndexToDirect:
		{
			int index = vertexUV->GetIndexArray().GetAt(inCtrlPointIndex);
			outUV.x = static_cast<float>(vertexUV->GetDirectArray().GetAt(index).mData[0]);
			outUV.y = static_cast<float>(vertexUV->GetDirectArray().GetAt(index).mData[1]);
		}
		break;

		default:
			throw std::exception("Invalid Reference");
		}
		break;

	case FbxGeometryElement::eByPolygonVertex:
		switch (vertexUV->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			outUV.x = static_cast<float>(vertexUV->GetDirectArray().GetAt(inVertexCounter).mData[0]);
			outUV.y = static_cast<float>(vertexUV->GetDirectArray().GetAt(inVertexCounter).mData[1]);
		}
		break;

		case FbxGeometryElement::eIndexToDirect:
		{
			int index = vertexUV->GetIndexArray().GetAt(inVertexCounter);
			outUV.x = static_cast<float>(vertexUV->GetDirectArray().GetAt(index).mData[0]);
			outUV.y = static_cast<float>(vertexUV->GetDirectArray().GetAt(index).mData[1]);
		}
		break;

		default:
			throw std::exception("Invalid Reference");
		}
		break;
	}
}

void FBXLoader::GetControlPoints(FbxNode* node, std::unordered_map<unsigned int, ControlPoint*>& pControlPoints)
{
	FbxMesh* currMesh = node->GetMesh();
	unsigned int ctrlPointCount = currMesh->GetControlPointsCount();
	for (unsigned int i = 0; i < ctrlPointCount; ++i)
	{
		ControlPoint* currCtrlPoint = new ControlPoint();
		XMFLOAT3 currPosition;
		currPosition.x = static_cast<float>(currMesh->GetControlPointAt(i).mData[0]);
		currPosition.y = static_cast<float>(currMesh->GetControlPointAt(i).mData[1]);
		currPosition.z = static_cast<float>(currMesh->GetControlPointAt(i).mData[2]);
		currCtrlPoint->mPosition = currPosition;
		pControlPoints[i] = currCtrlPoint;
	}
}

void FBXLoader::GetNode(FbxNode* & final_node, FbxNodeAttribute::EType attr, FbxNode* node = nullptr)
{
	if (node->GetNodeAttribute())
	{
		if (node->GetNodeAttribute()->GetAttributeType() == attr)
		{
			final_node = node;
			return;
		}
	}
	for (int i = 0; i < node->GetChildCount(); ++i)
	{
		GetNode(final_node, attr, node->GetChild(i));
	}
}

void FBXLoader::Optimize(std::vector<BlendingVertex> &mVertices, std::vector<Triangle>& mTriangles)
{
	// First get a list of unique vertices
	std::vector<BlendingVertex> uniqueVertices;
	for (unsigned int i = 0; i < mTriangles.size(); ++i)
	{
		for (unsigned int j = 0; j < 3; ++j)
		{
			// If current vertex has not been added to
			// our unique vertex list, then we add it
			if (FindVertex(mVertices[i * 3 + j], uniqueVertices) == -1)
			{
				uniqueVertices.push_back(mVertices[i * 3 + j]);
			}
		}
	}

	// Now we update the index buffer
	for (unsigned int i = 0; i < mTriangles.size(); ++i)
	{
		for (unsigned int j = 0; j < 3; ++j)
		{
			mTriangles[i].mIndices[j] = FindVertex(mVertices[i * 3 + j], uniqueVertices);
		}
	}

	mVertices.clear();
	mVertices = uniqueVertices;
	uniqueVertices.clear();

}

int FBXLoader::FindVertex(const BlendingVertex& inTargetVertex, const std::vector<BlendingVertex>& uniqueVertices)
{
	for (unsigned int i = 0; i < uniqueVertices.size(); ++i)
	{
		if (inTargetVertex == uniqueVertices[i])
		{
			return i;
		}
	}

	return -1;
}

FbxAMatrix FBXLoader::GetBindPose(FbxNode* mesh_node, FbxCluster* & cluster)
{
	FbxVector4 translation = mesh_node->GetGeometricTranslation(FbxNode::eSourcePivot);
	FbxVector4 rotation = mesh_node->GetGeometricRotation(FbxNode::eSourcePivot);
	FbxVector4 scaling = mesh_node->GetGeometricScaling(FbxNode::eSourcePivot);
	FbxAMatrix mesh_geometry_transform = FbxAMatrix(translation, rotation, scaling);

	FbxAMatrix bind_pose_transform;
	cluster->GetTransformLinkMatrix(bind_pose_transform);

	FbxAMatrix cluster_transform;
	cluster->GetTransformMatrix(cluster_transform);

	FbxAMatrix result = mesh_geometry_transform * cluster_transform.Inverse() * bind_pose_transform;

	return result;
}

//void FBXLoader::ProcessSkeletonHierarchy(Skeleton* & mSkeleton, FbxNode* inRootNode)
//{
//
//	for (int childIndex = 0; childIndex < inRootNode->GetChildCount(); ++childIndex)
//	{
//		FbxNode* currNode = inRootNode->GetChild(childIndex);
//		ProcessSkeletonHierarchyRecursively(mSkeleton, currNode, 0, 0, -1);
//	}
//}

//void FBXLoader::ProcessSkeletonHierarchyRecursively(Skeleton* & mSkeleton, FbxNode* inNode, int inDepth, int myIndex, int inParentIndex)
//{
//	if (inNode->GetNodeAttribute() && inNode->GetNodeAttribute()->GetAttributeType() && inNode->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eSkeleton)
//	{
//		Bone currJoint;
//		mSkeleton->joints[0]->bones.push_back(currJoint);
//	}
//	for (int i = 0; i < inNode->GetChildCount(); i++)
//	{
//		ProcessSkeletonHierarchyRecursively(mSkeleton, inNode->GetChild(i), inDepth + 1, mSkeleton->joints[0]->bones.size(), myIndex);
//	}
//}

void FBXLoader::ProcessGeometry(std::unordered_map<unsigned int, ControlPoint*> mControlPoints, Skeleton* & mSkeleton, FbxNode* inNode, unsigned int &triCount, std::vector<Triangle> &triIndices, std::vector<BlendingVertex> &verts, bool hasAnimation, FbxScene* & mScene, std::vector<Bone> & bind_pose, unsigned int & vertCount)
{

	if (inNode->GetNodeAttribute())
	{
		switch (inNode->GetNodeAttribute()->GetAttributeType())
		{
		case FbxNodeAttribute::eMesh:
			GetControlPoints(inNode, mControlPoints);
			if (hasAnimation)
			{
				LoadBonesandAnimations(inNode, mScene, mSkeleton, bind_pose, mControlPoints);
			}
			LoadMesh(inNode, mControlPoints, triCount, triIndices, verts, vertCount);
			break;
		}
	}

	for (int i = 0; i < inNode->GetChildCount(); ++i)
	{
		ProcessGeometry(mControlPoints, mSkeleton, inNode->GetChild(i), triCount, triIndices, verts, hasAnimation, mScene, bind_pose, vertCount);
	}
}

bool FBXLoader::ExportMeshFBX(const char* filename, const char* outFileNameMesh)
{
	FbxManager* mManager = nullptr;
	FbxScene* mScene = nullptr;
	InitializeSdkObjects(mManager, mScene);

}

bool FBXLoader::ExportFBX(const char* filename, const char* outFileNameMesh, const char* outFileNameBind, const char* outFileNameAnimations)
{
	FbxManager* mManager = nullptr;
	FbxScene* mScene = nullptr;
	bool bReturn = false;
	bool mHasAnimation = true;
	InitializeSdkObjects(mManager, mScene);
	if (mManager)
	{
		bReturn = LoadScene(mManager, mScene, filename);
		FbxGeometryConverter* converter = new FbxGeometryConverter(mManager);
		converter->Triangulate(mScene, true);
		Skeleton* mSkeleton = new Skeleton();
		std::vector<Bone> bind_pose;
		unsigned int vertCount = 0;
		/*ProcessSkeletonHierarchy(mSkeleton, mScene->GetRootNode());
		if (mSkeleton->joints.empty())
		{
			mHasAnimation = false;
		}*/
		std::unordered_map<unsigned int, ControlPoint*> mControlPoints;
		unsigned int TriCount;
		std::vector<Triangle> tris;
		std::vector<BlendingVertex> verts;
		ProcessGeometry(mControlPoints, mSkeleton, mScene->GetRootNode(), TriCount, tris, verts, mHasAnimation, mScene, bind_pose, vertCount);
		//Optimize(verts, tris);
		unsigned int KeyFrameCount = (unsigned int)mSkeleton->joints.size();
		unsigned int bindposeCount = (unsigned int)bind_pose.size();
		//Write out to File
		FILE* file = nullptr;
		fopen_s(&file, outFileNameMesh, "wb");
		if (file)
		{
			//int temp = sizeof(KeyFrame*);
			fwrite(&TriCount, sizeof(unsigned int), 1, file);
			fwrite(&vertCount, sizeof(unsigned int), 1, file);
			for (unsigned int i = 0; i < TriCount; ++i)
			{
				fwrite(&tris[i].mIndices[0], sizeof(unsigned int), 1, file);
				fwrite(&tris[i].mIndices[2], sizeof(unsigned int), 1, file);
				fwrite(&tris[i].mIndices[1], sizeof(unsigned int), 1, file);
			}
			for (unsigned int i = 0; i < vertCount; ++i)
			{
				unsigned int blendInfoSize = (unsigned int)verts[i].mVertexBlendInfos.size();
				fwrite(&verts[i].mPosition, sizeof(float), 3, file);
				fwrite(&verts[i].mNormal, sizeof(float), 3, file);
				fwrite(&verts[i].mUV, sizeof(float), 2, file);
				fwrite(&verts[i].mTangent, sizeof(float), 3, file);
				fwrite(&blendInfoSize, sizeof(unsigned int), 1, file);
				for (unsigned int j = 0; j < blendInfoSize; ++j)
				{
					fwrite(&verts[i].mVertexBlendInfos[j].mBlendIndex, sizeof(unsigned int), 1, file);
					fwrite(&verts[i].mVertexBlendInfos[j].mBlendWeight, sizeof(double), 1, file);
				}
			}
			

			fclose(file);
		}
		else
			LogError();

		file = nullptr;
		fopen_s(&file, outFileNameAnimations, "wb");
		if (file)
		{
			fwrite(&KeyFrameCount, sizeof(unsigned int), 1, file);
			for (int i = 0; i < mSkeleton->joints.size(); ++i)
			{
				unsigned int bone_size = (unsigned int)mSkeleton->joints[i]->bones.size();
				fwrite(&bone_size, sizeof(unsigned int), 1, file);
				fwrite(&mSkeleton->joints[i]->bones[0], sizeof(Bone), mSkeleton->joints[i]->bones.size(), file);
				fwrite(&mSkeleton->joints[i]->Time, sizeof(float), 1, file);
			}


			fclose(file);
		}
		else
			LogError();

		file = nullptr;
		fopen_s(&file, outFileNameBind, "wb");
		if (file)
		{
			fwrite(&bindposeCount, sizeof(unsigned int), 1, file);

			fwrite(&bind_pose[0], sizeof(Bone), bindposeCount, file);


			fclose(file);
		}
		else
			LogError();

		delete converter;
		//DestroySdkObjects(mManager);
		mScene->Destroy();
		mManager->Destroy();

		tris.clear();

		verts.clear();

		for (int i = 0; i < mSkeleton->joints.size(); ++i)
		{
			mSkeleton->joints[i]->bones.clear();
		}
		mSkeleton->joints.clear();
	}
	return bReturn;
}

//unsigned int FBXLoader::FindJointIndexUsingName(Skeleton* & mSkeleton, const std::string& inJointName)
//{
//	for (unsigned int i = 0; i < mSkeleton->joints.size(); ++i)
//	{
//		for (unsigned int j = 0; j < mSkeleton->joints[i]->bones.size(); ++j)
//		{
//			if (mSkeleton->joints[i]->bones[j].name == inJointName)
//			{
//				return i;
//			}
//		}
//	}
//
//	throw std::exception("Skeleton information in FBX file is corrupted.");
//}


XMFLOAT4X4 FBXLoader::FBXMatrixtoXMMatrix(FbxMatrix mat)
{
	XMFLOAT4X4 retMat;
	for (unsigned int i = 0; i < 4; ++i)
	{
		for (unsigned int j = 0; j < 4; ++j)
		{
			retMat.m[i][j] = (float)mat[i][j];
			if (j == 2)
			{
				retMat.m[i][j] *= -1;
			}
			if (i == 2)
			{
				retMat.m[i][j] *= -1;
			}
		}
	}
	return retMat;
}

void FBXLoader::GetTangent(FbxMesh* inMesh, int inCtrlPointIndex, int inVertexCounter, XMFLOAT3& outTangent)
{
	if (inMesh->GetElementTangentCount() < 1)
	{
		//throw std::exception("Invalid Tangent Number");
		outTangent = { 0,0,0 };
		return;
	}

	FbxGeometryElementTangent* vertexTangent = inMesh->GetElementTangent(0);
	switch (vertexTangent->GetMappingMode())
	{
	case FbxGeometryElement::eByControlPoint:
		switch (vertexTangent->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			outTangent.x = static_cast<float>(vertexTangent->GetDirectArray().GetAt(inCtrlPointIndex).mData[0]);
			outTangent.y = static_cast<float>(vertexTangent->GetDirectArray().GetAt(inCtrlPointIndex).mData[1]);
			outTangent.z = static_cast<float>(vertexTangent->GetDirectArray().GetAt(inCtrlPointIndex).mData[2]);
		}
		break;

		case FbxGeometryElement::eIndexToDirect:
		{
			int index = vertexTangent->GetIndexArray().GetAt(inCtrlPointIndex);
			outTangent.x = static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[0]);
			outTangent.y = static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[1]);
			outTangent.z = static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[2]);
		}
		break;

		default:
			throw std::exception("Invalid Reference");
		}
		break;

	case FbxGeometryElement::eByPolygonVertex:
		switch (vertexTangent->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			outTangent.x = static_cast<float>(vertexTangent->GetDirectArray().GetAt(inVertexCounter).mData[0]);
			outTangent.y = static_cast<float>(vertexTangent->GetDirectArray().GetAt(inVertexCounter).mData[1]);
			outTangent.z = static_cast<float>(vertexTangent->GetDirectArray().GetAt(inVertexCounter).mData[2]);
		}
		break;

		case FbxGeometryElement::eIndexToDirect:
		{
			int index = vertexTangent->GetIndexArray().GetAt(inVertexCounter);
			outTangent.x = static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[0]);
			outTangent.y = static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[1]);
			outTangent.z = static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[2]);
		}
		break;

		default:
			throw std::exception("Invalid Reference");
		}
		break;
	}
}

