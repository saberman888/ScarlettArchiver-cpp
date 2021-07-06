#include "Scarlett/Client/PushShift.hpp"
#include "Scarlett/Client/RateTracker.hpp"
#include "Scarlett/Internal/Helpers.hpp"
#include <fstream>
#include <iostream>
#include <exception>
#include <ctime>
#include <chrono>
#include <thread>
#include <cpprest/http_client.h>
#include <vector>
namespace Scarlett::Client {
	RateTracker& GetRateLimiter()
	{
		static RateTracker rt{ 120 };
		return rt;
	}

	HttpResponse PushShiftGet(std::string endpoint, const std::map<std::string, std::string>& URLComponents)
	{
		web::uri_builder b;

		b.set_scheme(utility::conversions::to_string_t("https"));
		b.set_host(utility::conversions::to_string_t("apiv2.pushshift.io"));
		b.set_path(conv::to_string_t(endpoint));
		b.append_query(conv::to_string_t(GenerateParamData(URLComponents)));

		auto r = web::http::http_request(web::http::methods::GET);
		r.set_request_uri(b.to_uri());

		auto rt = GetRateLimiter();
		return rt.Send(b.to_uri(), r);
	}

	
	
	HttpResponse PushShift::SearchSubmissions(const std::map<std::string, std::string>& URLComponents)
	{
		return PushShiftGet("/reddit/submission/search", URLComponents);
	}
	HttpResponse PushShift::SearchComments(const std::map<std::string, std::string>& URLComponents)
	{
		return PushShiftGet("/reddit/search/comment", URLComponents);
	}
};
