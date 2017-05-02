#include "SimpleFileIO.h"
#include <fstream>

SimpleFileIO::SimpleFileIO(std::string _Filename)
{
	Filename = _Filename;
}

SimpleFileIO::~SimpleFileIO()
{
	//Do nothing!
}

void SimpleFileIO::ReadFileToEnd()
{
	std::fstream File;
	File.open(Filename, std::ios::in | std::ios::binary);
	if (!File.is_open())
		return;
	char Char;
	while (!File.eof())
	{
		File.read(&Char, 1);
		if (!File.eof())
			ByteVector.push_back(Char);
	}
	File.close();
}

std::vector<char>* SimpleFileIO::GetByteVectorPTR()
{
	return &ByteVector;
}
