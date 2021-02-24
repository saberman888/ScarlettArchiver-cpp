#pragma once

#include "BasicRequest.hpp"
#include "nlohmann/json.hpp"
#include <vector>
#include <string>
#include <map>
#include <iosfwd>
#include <exception>
#include <ctime>
#include <chrono>
#include <thread>

typedef std::map<std::string, std::string> StringMap;

namespace PushShift {
		nlohmann::json SearchSubmissions(const std::map<std::string, std::string>& URLComponents);
		nlohmann::json SearchComments(const std::map<std::string, std::string>& URLComponents);
};


