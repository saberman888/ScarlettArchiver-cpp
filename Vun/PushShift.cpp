#include "PushShift.hpp"
#include <fstream>
#include <iostream>
namespace Vun::PushShift {

	static Response PushShiftGet(std::string endpoint, const std::map<std::string, std::string>& URLComponents)
	{
		static HttpClient::http_client client(web::uri(conv::to_string_t("api.pushshift.io")));

		web::uri_builder builder(conv::to_string_t(endpoint));

		builder.append_query(conv::to_string_t(Vun::Helpers::GenerateParamData(URLComponents)));
		auto result = client.request(web::http::methods::GET, builder.to_string()).get();
		return result;
	}
	
	Response SearchSubmissions(const std::map<std::string, std::string>& URLComponents)
	{
		return PushShiftGet("/reddit/submission/search", URLComponents);
	}
	Response SearchComments(const std::map<std::string, std::string>& URLComponents)
	{
		return PushShiftGet("/reddit/search/comment", URLComponents);
	}
};
