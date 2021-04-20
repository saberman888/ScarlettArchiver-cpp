#include "PushShift.hpp"
#include <fstream>
#include <iostream>
namespace Scarlett::Client {

	HttpResponse PushShift::PushShiftGet(std::string endpoint, const std::map<std::string, std::string>& URLComponents)
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

	const RateTracker& PushShift::GetRateLimiter()
	{
		static RateTracker rt{ 120 };
		return rt;
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
