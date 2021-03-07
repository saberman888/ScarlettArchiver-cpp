#pragma once

#include <iomanip>
#include <filesystem>
#include <fstream>
#include <cpprest/http_client.h>
#include "StringOps.hpp"
/*
* A header file for misc functions and single functions that don't fit in any other header file
*/
namespace HttpClient = web::http::client;
namespace conv = utility::conversions;
namespace JSON = web::json;

namespace ScarlettArchiver
{	
	/**
	* just like the normal strptime on Linux. I just needed an independent implementation because, MSVC doesn't have one
	* @author Sai Vazquez
	* @params Date in string format, the date's format and a struct tm*
	*/
	char* strptime(const char* s, const char* f, struct tm* tm);

	web::http::http_response Download(const std::string URL);

	void Write(const JSON::value& src, const std::filesystem::path dest, const std::string filename);
	void Write(const std::string& buff, std::string filename);

}
