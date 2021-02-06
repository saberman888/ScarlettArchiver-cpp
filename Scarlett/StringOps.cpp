#include "StringOps.hpp"
namespace ScarlettArchiver {
	std::vector<std::string> splitString(std::string data, char delimeter)
	{
		std::string temp;
		std::vector<std::string> returnList;
		std::stringstream ss(data);

		while (std::getline(ss, temp, delimeter))
		{
			returnList.push_back(temp);
		}
		return returnList;
	}

	std::string SearchAndReplace(std::string Input, const std::string ToBeReplaced, const std::string Replacement)
	{
		if (Input.find(ToBeReplaced) != std::string::npos)
		{
			Input.replace(Input.find(ToBeReplaced), ToBeReplaced.size(), Replacement);
		}
		return Input;
	}

}