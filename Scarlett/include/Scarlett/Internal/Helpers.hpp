#pragma once

#include "Types.hpp"

/*
* A header file for misc functions and single functions that don't fit in any other header file
*/
namespace Scarlett
{
	namespace conv = utility::conversions;
	namespace JSON = web::json;
	/**
	* just like the normal strptime on Linux. I just needed an independent implementation because, MSVC doesn't have one
	* @author Sai Vazquez
	* @params Date in string format, the date's format and a struct tm*
	*/
	SCDLL char* strptime(const char* s, const char* f, struct tm* tm);

	SCDLL std::string formatTime(tm& _time, std::string format);

	// Converts anything in data to a var1=data&var2=data format
	SCDLL std::string GenerateParamData(const StringMap& data);

	SCDLL bool contains(const std::string& lhs, const std::string rhs);

	SCDLL HttpResponse Download(URI source);

	SCDLL HttpResponse Download(std::string&& source);
}
