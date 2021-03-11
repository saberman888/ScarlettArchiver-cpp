#pragma once

#include "Core/StringOps.hpp"
#include "Core/exceptions.hpp"
#include <cpprest/http_client.h>
#include <cpprest/json.h>
#include <cpprest/http_msg.h>
#include <cpprest/http_headers.h>
#include <cpprest/oauth2.h>

typedef web::http::http_response Response;
namespace JSON = web::json;
namespace HttpClient = web::http::client;
namespace Http = web::http;
namespace conv = utility::conversions;

namespace Scarlett::Media::ImgurAccess
{
	inline bool IsAlbum(std::string URL)
	{
		return (URL.find("https://imgur.com/a/") != std::string::npos);
	}

	inline bool IsImgurLink(std::string URL)
	{
		return (URL.find("https://imgur.com/", 0) != std::string::npos);
	}

	inline static std::string GetHash(std::string URL)
	{
		return Scarlett::splitString(URL, '/').back();
	}

	inline bool IsDirect(std::string URL)
	{
		return URL.find("i.imgur.com/", 0) != std::string::npos;
	}

	std::vector<std::string> GetAlbum(std::string AlbumHash, std::string ClientId);
	std::string GetImage(std::string ImageHash, std::string ClientId);
	std::vector<std::string> ResolveAlbumURLs(std::string URL, std::string ClientId);
}
