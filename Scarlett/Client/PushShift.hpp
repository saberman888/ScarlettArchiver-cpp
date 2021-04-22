#pragma once

#include "../Internal/Helpers.hpp"
#include "../Internal/IntervalTracker.hpp"
#include <cpprest/http_client.h>
#include <vector>
#include <string>
#include <map>
#include <iosfwd>
#include <exception>
#include <ctime>
#include <chrono>
#include <thread>

namespace Scarlett::Client {
	class PushShift {
	public:
		static HttpResponse SearchSubmissions(const std::map<std::string, std::string>& URLComponents);
		static HttpResponse SearchComments(const std::map<std::string, std::string>& URLComponents);
	private:
		static HttpResponse PushShiftGet(std::string endpoint, const std::map<std::string, std::string>& URLComponents);
		static const RateTracker& GetRateLimiter();
	};
};


