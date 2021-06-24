#include "Scarlett/Media/Imgur.hpp"

namespace Scarlett::Media::ImgurAccess
{
	const String GetHash(const URI& uri)
	{
		if (auto hash = uri.query(); hash[0] == SCARLETT_WIDEN('/'))
		{
			return uri.query().substr(1);
		}
		else {
			return uri.query();
		}
	}

	static HttpResponse ImgurGet(const String& endpoint, const String& ClientId)
	{
		auto FullUri = URI("https://api.imgur.com/"_u + endpoint);
		HttpClient client(FullUri.authority());

		HttpRequest req(HttpMethod::GET);
		auto headers = req.headers();

		headers.add("Authorization"_u, " CLIENT-ID "_u + ClientId);
		req.set_request_uri(FullUri.query());
		return client.request(req).get();
	}

	static const String ParseImage(const JSON::value& json)
	{
		try {
			if (json.has_field("data"_u))
			{
				return json.at("data"_u).at("link"_u).as_string();
			}
		}
		catch (JSON::json_exception& e) {
			scarlettNestedThrow("Failed to parse JSON from Imgur Link, " + std::string(e.what()));
		}
		return String();
	}

	static const std::vector<String> ParseAlbum(const JSON::value& json)
	{
		std::vector<String> URLs;
		try {
			if (json.at("data"_u).at("images"_u).is_array())
			{
				JSON::array images = json.at("data"_u).at("images"_u).as_array();
				for (auto& elem : images)
				{
					URLs.push_back(elem.as_string());
				}
			}
		}
		catch (JSON::json_exception& e) {
			scarlettNestedThrow("Failed to parse JSON from Imgur Link, " + std::string(e.what()));
		}
		return URLs;
	}

	const String GetImage(const String& ImageHash, const String& ClientId)
	{
		String endpoint = "/3/image/"_u + GetHash(ImageHash);
		auto result = ImgurGet(endpoint, ClientId);
		if (result.status_code() == 200)
		{
			return ParseImage(result.extract_json().get());
		}
		else {
			scarlettThrow("Failed to get Imgur image link from API: " + toString(ImageHash));
		}
		return String();
	}

	const std::vector<String> GetAlbum(const URI& uri, const String& ClientId)
	{

		if (IsAlbum(uri))
		{
			auto result = ImgurGet("/3/album/"_u + GetHash(uri) + "/images"_u, ClientId);
			if (result.status_code() == 200)
			{
				return ParseAlbum(result.extract_json().get());
			}
			else {
				scarlettThrow("Failed to get Imgur album data from API: " + toString(uri.to_string()));
			}
		}
		return std::vector<String>();
	}
}
