#pragma once

#include "../Internal/Helpers.hpp"
#include "../Internal/Exceptions.hpp"
#include "../Client/RateTracker.hpp"
#include <cpprest/http_client.h>
#include <cpprest/json.h>
#include <cpprest/http_msg.h>
#include <cpprest/http_headers.h>
#include <cpprest/oauth2.h>
#include <regex>

namespace Scarlett::Media::ImgurAccess
{
	inline bool IsAlbum(const URI& uri)
	{
		return std::regex_match(toString(uri.to_string()), std::regex("https?://imgur.com/a/[A-Za-z0-9]+"));
	}

	inline bool IsImgurLink(const URI& uri)
	{
		return std::regex_match(toString(uri.to_string()), std::regex("https?://(i.)?imgur.com(/|/a/)?[A-Za-z0-9]+"));
	}

	inline bool IsDirect(const URI& uri)
	{
		return std::regex_match(toString(uri.to_string()), std::regex("https?://i.imgur.com/[A-Za-z0-9]+"));
	}

	static const String GetHash(const URI& uri);
	const std::vector<String> GetAlbum(const URI& URL, const String& ClientId);
	const String GetImage(const String& ImageHash, const String& ClientId);

}
