#include "Scarlett/Internal/Helpers.hpp"

namespace Scarlett
{
	char* strptime(const char* s, const char* f, struct tm* tm) {
		std::istringstream input(s);
		input.imbue(std::locale(setlocale(LC_ALL, nullptr)));
		input >> std::get_time(tm, f);
		if (input.fail()) {
			return nullptr;
		}
		return (char*)(s + input.tellg());
	}

	std::string formatTime(tm& _time, std::string format)
	{
		std::stringstream ss;
		ss << std::put_time(&_time, format.c_str());
		return ss.str();
	}

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

	utility::string_t operator""_u(const char* source, size_t csize)
	{
		return utility::conversions::to_string_t(std::string(source, csize));
	}
}