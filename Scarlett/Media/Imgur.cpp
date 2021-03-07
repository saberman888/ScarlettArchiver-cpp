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
				link = ToU8String(json.at("data"_u).at("link"_u).as_string());
			}
		}
		catch (JSON::json_exception& e) {
			scarlettNestedThrow("Failed to parse JSON from Imgur Link, " + std::string(e.what()));
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
			scarlettNestedThrow("Failed to parse JSON from Imgur Link, " + std::string(e.what()));
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
			scarlettThrow("Failed to get Imgur image link from API: " + ImageHash);
		}
		return std::string();
	}

	std::vector<std::string> GetAlbum(std::string Album, std::string ClientId)
	{
		std::string endpoint = "/3/album/" + GetHash(Album) + "/images";
		std::vector<std::string> Images;
		auto result = ImgurGet(endpoint, ClientId);
		if (result.status_code() == 200)
		{
			auto links = ParseAlbum(result.extract_json().get());
			return links;
		}
		else {
			scarlettThrow("Failed to get Imgur album data from API: " + Album);
		}
		return std::vector<std::string>();
	}

	std::vector<std::string> ResolveAlbumURLs(std::string URL, std::string ClientId)
	{
		std::vector<std::string> Images;
		if (IsImgurLink(URL) && !IsDirect(URL))
		{
			if (IsAlbum(URL))
			{
				Images = ImgurAccess::GetAlbum(URL, ClientId);
			}
		}
		return Images;
	}
}
