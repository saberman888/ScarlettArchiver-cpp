#include "PushShift.hpp"
#include <fstream>
#include <iostream>
namespace PushShift {

	static pplx::task<web::http::http_response> PushShiftGet(std::string endpoint, const std::map<std::string, std::string>& URLComponents)
	{
		/*BR::BasicRequest<BR::RequestType::GET> handle({
			"https", "api.pushshift.io", 80, endpoint + Vun::Helpers::GenerateParamData(URLComponents)
			});
        auto result = handle.SendRequest();
        handle.Cleanup();
		return *result;*/
		HttpClient::http_client req(U("api.pushshift.io"));
		web::uri_builder builder(conv::to_string_t(endpoint));
		builder.append_query(conv::to_string_t(Vun::Helpers::GenerateParamData(URLComponents)));
		return req.request(web::http::methods::GET, builder.to_string());
	}
	
	nlohmann::json SearchSubmissions(const std::map<std::string, std::string>& URLComponents)
	{
		auto result = PushShiftGet("/reddit/submission/search", URLComponents);
		auto resultDone = result.get();

	}
	nlohmann::json SearchComments(const std::map<std::string, std::string>& URLComponents)
	{
		auto result = PushShiftGet("/reddit/search/comment", URLComponents);
		if (result.HttpState == 200)
		{
			return nlohmann::json::parse(result.buffer);
		}
		else {
			std::cerr << result.HttpState << ": " << result.message << std::endl;
			throw std::runtime_error("Error failed to retrieve data");
		}
		return nlohmann::json();
	}
};
