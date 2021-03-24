#include "PushShift.hpp"
#include <fstream>
#include <iostream>
namespace Vun::PushShift {

	static Response PushShiftGet(std::string endpoint, const std::map<std::string, std::string>& URLComponents)
	{
		web::uri_builder b(conv::to_string_t("https://api.pushshift.io"));
		b.set_path(conv::to_string_t(endpoint));
		b.append_query(conv::to_string_t(Vun::Helpers::GenerateParamData(URLComponents)));

		HttpClient::http_client client(b.to_uri());

		auto result = client.request(web::http::methods::GET);
		return result.get();
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
