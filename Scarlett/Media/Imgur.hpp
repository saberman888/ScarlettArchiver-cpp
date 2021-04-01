#pragma once

#include "../Core/StringOps.hpp"
#include "../Core/exceptions.hpp"
#include <cpprest/http_client.h>
#include <cpprest/json.h>
#include <cpprest/http_msg.h>
#include <cpprest/http_headers.h>
#include <cpprest/oauth2.h>
#include <regex>

typedef web::http::http_response Response;
namespace JSON = web::json;
namespace HttpClient = web::http::client;
namespace Http = web::http;
namespace conv = utility::conversions;

namespace Scarlett::Media::ImgurAccess
{
	inline bool IsAlbum(std::string URL)
	{
		return std::regex_match(URL, std::regex("https?://imgur.com/a/[A-Za-z0-9]+"));
	}

	inline bool IsImgurLink(std::string URL)
	{
		return std::regex_match(URL, std::regex("https?://(i.)?imgur.com/(a/)?[A-Za-z0-9]+"));
	}

	inline static std::string GetHash(std::string URL)
	{
		return Scarlett::splitString(URL, '/').back();
	}

	inline bool IsDirect(std::string URL)
	{
		return std::regex_match(URL, std::regex("https?://i.imgur.com/[A-Za-z0-9]+"));
	}

	std::vector<std::string> GetAlbum(std::string URL, std::string ClientId);
	std::string GetImage(std::string ImageHash, std::string ClientId);

}
