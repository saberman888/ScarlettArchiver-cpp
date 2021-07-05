#pragma once

#include "../Internal/Types.hpp"
#include <string>
#include <map>

namespace Scarlett::Client {
    class SCDLL PushShift {
	public:
		static HttpResponse SearchSubmissions(const std::map<std::string, std::string>& URLComponents);
		static HttpResponse SearchComments(const std::map<std::string, std::string>& URLComponents);
	};
};


