#pragma once

#include <iomanip>
#include <filesystem>
#include <fstream>
#include <cpprest/http_client.h>

/*
* A header file for misc functions and single functions that don't fit in any other header file
*/
namespace Scarlett
{	
	namespace HttpClient = web::http::client;
	namespace conv = utility::conversions;
	namespace JSON = web::json;
	using StringMap = std::map<std::string, std::string>;

	/**
	* just like the normal strptime on Linux. I just needed an independent implementation because, MSVC doesn't have one
	* @author Sai Vazquez
	* @params Date in string format, the date's format and a struct tm*
	*/
	char* strptime(const char* s, const char* f, struct tm* tm);

	std::string formatTime(tm& _time, std::string format);

	// Converts anything in data to a var1=data&var2=data format
	std::string GenerateParamData(const StringMap& data);

	inline bool contains(const std::string& lhs, const std::string rhs)
	{
		return (lhs.rfind(rhs) != std::string::npos);
	}
	std::vector<std::string> splitString(std::string data, char delimeter);
	std::string SearchAndReplace(std::string Input, const std::string ToBeReplaced, const std::string Replacement);

	inline std::string u8(const utility::string_t& source) {
		return utility::conversions::to_utf8string(source);
	}

	utility::string_t operator ""_u(const char* source, size_t csize);

}
