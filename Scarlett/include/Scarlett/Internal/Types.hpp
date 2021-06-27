#pragma once

#include <cpprest/http_client.h>
#include <cpprest/json.h>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>

#ifdef _WIN32
#define __SCARLETT_USE_WIDE_STRINGS
#endif

namespace Scarlett
{

	// cpprestsdk aliases
	using HttpResponse = web::http::http_response;
	using HttpRequest = web::http::http_request;
	using HttpClient = web::http::client::http_client;
	using HttpClientConfig = web::http::client::http_client_config;
	using HttpMethod = web::http::methods;
	using URI = web::uri;
	using StatusCode = web::http::status_code;
	using JsonValue = web::json::value;
	namespace OAuth2 = web::http::oauth2::experimental;
	namespace conv = utility::conversions;
	namespace Json = web::json;

	// Time
	using TimePoint = std::chrono::time_point<std::chrono::system_clock>;
	using Millisecond = std::chrono::milliseconds;

	// String related stuff without the utility namespace
	using StringMap = std::map<std::string, std::string>;
	using String = utility::string_t;
	using Char = utility::char_t;
	using ofstream = utility::ofstream_t;
	using ifstream = utility::ifstream_t;
	using istringstream = utility::istringstream_t;
	using ostringstream = utility::ostringstream_t;
	using stringstream = utility::stringstream_t;

	inline String toScarlettString(const std::string source) {
#ifdef __SCARLETT_USE_WIDE_STRINGS
		return conv::to_string_t(source);
#else
		return source;
#endif
	}

	inline std::string toString(const String& source) {
#ifdef __SCARLETT_USE_WIDE_STRINGS
		return utility::conversions::to_utf8string(source);
#else
		return source;
#endif
	}

	inline String operator""_u(const char* source, size_t csize)
	{
		return utility::conversions::to_string_t(std::string(source, csize));
	}

#ifdef __SCARLETT_USE_WIDE_STRINGS
#define SCARLETT_WIDEN(x) _XPLATSTR(x)
#else
#define SCARLETT_WIDEN(x) x
#endif
	template<class CharT, 
			 class Traits = std::char_traits<CharT>, 
			 class Allocator = std::allocator<CharT>>
	std::vector<std::basic_string<CharT, Traits, Allocator>> splitString(std::basic_string<CharT, Traits, Allocator> data, CharT delimeter)
	{
		decltype(data) temp;
		std::vector<decltype(data)> returnList;
		std::basic_stringstream<CharT, Traits, Allocator> ss(data);

		while (std::getline(ss, temp, delimeter))
		{
			returnList.push_back(temp);
		}
		return returnList;
	}

	template<class CharT, 
			 class Traits = std::char_traits<CharT>, 
			 class Allocator = std::allocator<CharT>>
	std::basic_string<CharT, Traits, Allocator> SearchAndReplace(std::basic_string<CharT, Traits, Allocator> Input, const std::basic_string<CharT, Traits, Allocator> ToBeReplaced, const std::basic_string<CharT, Traits, Allocator> Replacement)
	{
		if (Input.find(ToBeReplaced) != std::basic_string<CharT, Traits, Allocator>::npos)
		{
			Input.replace(Input.find(ToBeReplaced), ToBeReplaced.size(), Replacement);
		}
		return Input;
	}

	// Other
	using Size = utility::size64_t;
};