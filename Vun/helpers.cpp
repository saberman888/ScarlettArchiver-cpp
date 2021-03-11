#include "helpers.hpp"

namespace Vun::Helpers
{
	std::string GenerateParamData(const StringMap& data)
	{
		std::string returnData;
		for (std::map<std::string, std::string>::const_iterator it = data.begin(); it != data.end(); it++)
		{
			returnData.append(
				((it == data.begin()) ? "" : "&")
				+ it->first
				+ "="
				+ it->second
			);
		}
		return returnData;
	}
};