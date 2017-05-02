#ifndef __EXPORTER_HEADER_H__
#define __EXPORTER_HEADER_H__
//--------------------------------------------------------------------------------
//THis file should be included in the loader and the exporter
#include <memory.h>
#include <stdint.h>
#include <sys/stat.h>
#include <iostream>

#include <time.h>
//--------------------------------------------------------------------------------

//Change this when any changes are done to the struct ExporterHeader
#define EXPORTER_VERSION_NUMBER 1
//--------------------------------------------------------------------------------
namespace FileInfo {
	enum class FILE_TYPES : uint8_t { MESH, BIND_POSE, ANIMATION, NAV_MESH, ERROR_TYPE };
	enum class MODEL_TYPES : uint8_t { COLOR, TEXTURE, TEXTURE_LIT, NORMALMAP, NORMALMAP_ANIMATED, BASIC, MAX_TYPES };
	enum class INDEX_TYPES : uint8_t { INDEX32, INDEX16, TRI_STRIP };

	struct ExporterHeader {
		union {
			struct {
				uint32_t		numPoints;
				uint32_t		numIndex;
				uint32_t		vertSize;
				MODEL_TYPES		modelType;
				INDEX_TYPES		index;
			}mesh;

			struct {
				uint32_t		numBones;
				uint32_t		nameSize;
			}bind;

			struct {
				uint32_t		numBones;
				uint32_t		numFrames;
				float			startTime;
				float			endTime;
			}anim;

			struct {
				uint32_t		totalSize;
				uint32_t		rowSize;
				uint32_t		numTriangles;
				uint32_t		elementSize;
			}navMesh;
		};

		FILE_TYPES		file;
		uint32_t		version;
		//Do a check to make sure the file has not been updated since you last changed it
		time_t			updateTime;

		ExporterHeader() :file(FILE_TYPES::ERROR_TYPE) {}
		//used for writing out the header
		ExporterHeader(_In_ FILE_TYPES _type, _In_z_ const char * _FBXFileLocation) :file(_type), version(EXPORTER_VERSION_NUMBER) {
			navMesh.elementSize = -1;
			navMesh.numTriangles = -1;
			navMesh.rowSize = -1;
			navMesh.totalSize = -1;
			//May have to change this memset if any other structure gets larger than the biggest part of data
			memset(&mesh, 0, sizeof(mesh));
			//https://msdn.microsoft.com/query/dev12.query?appId=Dev12IDEF1&l=EN-US&k=k(wchar%2Fstat);k(stat);k(DevLang-C%2B%2B);k(TargetOS-Windows)&rd=true
			struct stat fileProperty;
			//Return value shown as an example but not used in this function
			int retVal = stat(_FBXFileLocation, &fileProperty);
			//save last modified time to the file
			updateTime = fileProperty.st_mtime;
			////If you want to see the time in a more readable format
			//struct tm time;               // create a time structure
			//gmtime_s(&time, &(fileProperty.st_mtime)); // Get the last modified time and put it into the time structure
		}
		//Purpose: Read the header and fill it out. Check for for binary file to make sure the importer will work with this file type
		//Return: true if the header is valid and filled outor false means the file must be exported again, for some reason
		//Note: _file will be nullptr if the function returns false
		bool ReadHeader(_Out_ FILE **_file, _In_z_ const char * _binFileLocation, _In_z_ const char * _FBXFileLocation) {
			//Check to see if the file has been exported yet
			fopen_s(_file, _binFileLocation, "rb");
			if (nullptr == *_file) return false;
			//Fill out this class
			fread(this, sizeof(*this), 1, *_file);
			//check to see if this file is the same version that wrote the file
			if (EXPORTER_VERSION_NUMBER != version) {
				fclose(*_file);
				*_file = nullptr;
				return false;
			}
			//Check to see if the fbx file has been updated after we exported the file
			struct stat fileProperty;
			stat(_FBXFileLocation, &fileProperty);
			if (updateTime != fileProperty.st_mtime) {
				fclose(*_file);
				*_file = nullptr;
				return false;
			}

			return true;
		}
	};
};
//--------------------------------------------------------------------------------
#endif //__EXPORTER_HEADER_H__