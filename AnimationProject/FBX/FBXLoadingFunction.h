#include "ExporterHeader.h"
#include "ObjectStructures.h"

#define DLLEXPORT __declspec(dllexport)
#ifdef IOS_REF
#undef  IOS_REF
#define IOS_REF (*(pManager->GetIOSettings()))
#endif

//DLLEXPORT bool LoadMesh(char* filename, FileInfo::FILE_TYPES type, unsigned int& triCount, std::vector<Triangle> &triIndices, std::vector<BlendingVertex> &verts, const char* & texName);
//DLLEXPORT bool LoadBones(char* filename, FileInfo::FILE_TYPES type, std::vector<Bone> &bones);
DLLEXPORT bool ExportFBX(const char* filename, const char * outFilenameMesh, const char* outFileNameBind, const char* outFileNameAnimations);
DLLEXPORT bool LoadMeshFBX(const char * filename, unsigned int & triCount, std::vector<unsigned int>& triIndices, std::vector<BlendingVertex>& verts);
DLLEXPORT bool LoadBonesFBX(const char * filenameBone, std::vector<Bone>& bind_pose);
DLLEXPORT bool LoadAnimationFBX(const char * fileNameAnimations, Skeleton *& mSkeleton);
DLLEXPORT bool LoadFBX(const char* filename, const char* filenameBone, const char* fileNameAnimations, unsigned int& triCount, std::vector<unsigned int>&triIndices, std::vector<BlendingVertex> &verts, Skeleton* & mSkeleton, std::vector<Bone> &bind_pose);
