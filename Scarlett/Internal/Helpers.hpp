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
	namespace conv = utility::conversions;
	namespace JSON = web::json;
	using StringMap = std::map<std::string, std::string>;
	using WideString = utility::string_t;
	using HttpResponse = web::http::http_response;
	using HttpRequest = web::http::http_request;
	using HttpClient = web::http::client::http_client;
	using HttpClientConfig = web::http::client::http_client_config;
	using Millisecond = std::chrono::milliseconds;
	using HttpMethod = web::http::methods;
	using URI = web::uri;
	using StatusCode = web::http::status_code;
	using Size = utility::size64_t;

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

	inline std::string u8(const WideString& source) {
		return utility::conversions::to_utf8string(source);
	}

	utility::string_t operator ""_u(const char* source, size_t csize);

	inline HttpResponse Download(URI source)
	{
		HttpClient cl(source);
		return cl.request(web::http::methods::GET).get();
	}

	inline HttpResponse Download(std::string&& source)
	{
		return Download(conv::to_string_t(std::forward<std::string>(source)));
	}

}
