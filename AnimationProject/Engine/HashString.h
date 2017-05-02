#pragma once
#include <map>

class HashString
{
	std::map<std::string, unsigned int> Lookup;
public:
	HashString();
	~HashString();
	//Uses a constant reference to a string to hash into the map and find an integer.
	unsigned int GetKey(const std::string & string);
};

