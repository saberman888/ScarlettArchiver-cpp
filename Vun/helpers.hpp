#pragma once
#include <vector>
#include <string>
#include <map>

namespace Vun::Helpers
{
	/*
	Some helper function that don't fit with the other header files and classes will be here
	*/


	using StringMap = std::map<std::string, std::string>;
	// Converts anything in data to a var1=data&var2=data format
	std::string GenerateParamData(const StringMap& data);

};
