#include "Imgur.hpp"

namespace Scarlett::Media::ImgurAccess
{
	static Response ImgurGet(std::string endpoint, std::string ClientId)
	{

		static HttpClient::http_client client(web::uri("https://api.imgur.com/"_u));

		Http::http_request req(Http::methods::GET);
		auto headers = req.headers();

		std::string lefthandHeader = " CLIENT-ID " + ClientId;

		headers.add("Authorization"_u, conv::to_string_t(lefthandHeader));
		req.set_request_uri(conv::to_string_t(endpoint));
		return client.request(req).get();
	}

	static std::string ParseImage(JSON::value json)
	{
		std::string link;
		try {
			if (json.has_field("data"_u))
			{
				link = u8(json.at("data"_u).at("link"_u).as_string());
			}
		}
		catch (JSON::json_exception& e) {
			auto msg = "Failed to parse JSON from Imgur Link, " + std::string(e.what());
			scarlettNestedThrow(msg);
		}
		return link;
	}
	static std::vector<std::string> ParseAlbum(JSON::value json)
	{
		std::vector<std::string> URLs;
		try {
			if (json.at("data"_u).at("images"_u).is_array())
			{
				JSON::array images = json.at("data"_u).at("images"_u).as_array();
				for (auto& elem : images)
				{
					auto link = conv::to_utf8string(elem.as_string());
					URLs.push_back(link);
				}
			}
		}
		catch (JSON::json_exception& e) {
			auto msg = "Failed to parse JSON from Imgur Link, " + std::string(e.what());
			scarlettNestedThrow(msg);
		}
		return URLs;
	}

	std::string GetImage(std::string ImageHash, std::string ClientId)
	{
		std::string endpoint = "/3/image/" + GetHash(ImageHash);
		auto result = ImgurGet(endpoint, ClientId);
		if (result.status_code() == 200)
		{
			auto link = ParseImage(result.extract_json().get());
			return link;
		}
		else {
			auto msg = "Failed to get Imgur image link from API: " + ImageHash;
			scarlettThrow(msg);
		}
		return std::string();
	}

	std::vector<std::string> GetAlbum(std::string URL, std::string ClientId)
	{

		if (IsAlbum(URL))
		{
			auto result = ImgurGet("/3/album/" + GetHash(URL) + "/images", ClientId);
			if (result.status_code() == 200)
			{
				return ParseAlbum(result.extract_json().get());
			}
			else {
				std::string msg = "Failed to get Imgur album data from API: " + URL;
				scarlettThrow(msg);
			}
		}
		return std::vector<std::string>();
	}
}
