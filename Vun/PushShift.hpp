#pragma once

#include "helpers.hpp"
#include "nlohmann/json.hpp"
#include <cpprest/http_client.h>
#include <ppltasks.h>
#include <vector>
#include <string>
#include <map>
#include <iosfwd>
#include <exception>
#include <ctime>
#include <chrono>
#include <thread>

typedef std::map<std::string, std::string> StringMap;
namespace HttpClient = web::http::client;
namespace OAuth2 = web::http::oauth2::experimental;
namespace conv = utility::conversions;

namespace PushShift {
		nlohmann::json SearchSubmissions(const std::map<std::string, std::string>& URLComponents);
		nlohmann::json SearchComments(const std::map<std::string, std::string>& URLComponents);
};


