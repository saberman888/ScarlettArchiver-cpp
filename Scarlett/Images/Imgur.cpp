#include "Imgur.hpp"

namespace ScarlettArchiver
{
	namespace ImgurAccess
	{
		static Response ImgurGet(std::string endpoint, std::string ClientId)
		{
			std::string URL = "https://api.imgur.com" + endpoint;

			static HttpClient::http_client client(L"https://api.imgur.com/");

			Http::http_request req(Http::methods::GET);
			auto headers = req.headers();

			std::string lefthandHeader = " CLIENT-ID " + ClientId;

			headers.add(L"Authorization", conv::to_string_t(lefthandHeader));
			req.set_request_uri(conv::to_string_t(endpoint));
			return client.request(req).get();
		}

		static std::string ParseImage(JSON::value json)
		{
			std::string link;
			try {
				if (json.has_field(L"data"))
				{
					link = conv::to_utf8string(json.at(L"data").at(L"link").as_string());
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
				if (json.at(L"data").at(L"images").is_array())
				{
					JSON::array images = json.at(L"data").at(L"images").as_array();
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
}
