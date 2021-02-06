#pragma once
#include <string>
#include <sstream>
#include <vector>

namespace ScarlettArchiver {
	inline bool contains(const std::string& lhs, const std::string rhs)
	{
		return (lhs.rfind(rhs) != std::string::npos);
	}
	std::vector<std::string> splitString(std::string data, char delimeter);
	std::string SearchAndReplace(std::string Input, const std::string ToBeReplaced, const std::string Replacement);
}
