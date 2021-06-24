#pragma once

#include <cpprest/http_client.h>

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
	namespace OAuth2 = web::http::oauth2::experimental;
	namespace conv = utility::conversions;

	// Time
	using TimePoint = std::chrono::time_point<std::chrono::system_clock>;
	using Millisecond = std::chrono::milliseconds;

	// String related stuff
	using StringMap = std::map<std::string, std::string>;
	using String = utility::string_t;

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

#ifdef __SCARLETT_USE_WIDE_STRINGS
#define WIDEN(x) _XPLATSTR(x)
#else
#define WIDEN(x) x
#endif
	// Other
	using Size = utility::size64_t;
};