#pragma once
#include <string>
#include <sstream>
#include <vector>
#include <cpprest/json.h>

namespace ScarlettArchiver {
	inline bool contains(const std::string& lhs, const std::string rhs)
	{
		return (lhs.rfind(rhs) != std::string::npos);
	}
	std::vector<std::string> splitString(std::string data, char delimeter);
	std::string SearchAndReplace(std::string Input, const std::string ToBeReplaced, const std::string Replacement);
	
	inline std::string ToU8String(const utility::string_t& source){
		return utility::conversions::to_utf8string(source);
	}

	utility::string_t operator ""_u(const char* source, size_t csize);


}
