#pragma once
#include <string>
#include <vector>
class SimpleFileIO
{
	std::string Filename;
	std::vector<char> ByteVector;
public:
	SimpleFileIO(std::string Filename);
	~SimpleFileIO();
	//Must be called or ByteVector will be empty.
	void ReadFileToEnd();
	//Returns a pointer to the byte vector filled out by ReadFileToEnd()
	std::vector<char>* GetByteVectorPTR();
};

