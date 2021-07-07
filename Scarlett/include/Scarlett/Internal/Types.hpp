#pragma once

#include "Scarlett_def.hpp"
#include <cpprest/http_client.h>
#include <cpprest/json.h>

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

#ifdef __SCARLETT_USE_WIDE_STRINGS
#define scout std::wcout;
#define scerr = std::wcerr;
#else
#define scout = std::cout;
#define scerr = std::cerr;
#endif

	SCDLL String toScarlettString(const std::string source);

	SCDLL std::string toString(const String& source);

	SCDLL String operator""_u(const char* source, size_t csize);

	template<class CharT,
		class Traits = std::char_traits<CharT>,
		class Allocator = std::allocator<CharT>>
		SCDLL std::vector<std::basic_string<CharT, Traits, Allocator>> splitString(std::basic_string<CharT, Traits, Allocator> data, CharT delimeter);

	template<class CharT,
		class Traits = std::char_traits<CharT>,
		class Allocator = std::allocator<CharT>>
		SCDLL std::basic_string<CharT, Traits, Allocator> SearchAndReplace(std::basic_string<CharT, Traits, Allocator> Input, const std::basic_string<CharT, Traits, Allocator> ToBeReplaced, const std::basic_string<CharT, Traits, Allocator> Replacement);

	// Other
	using Size = utility::size64_t;
};
