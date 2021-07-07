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
	SCDLL bool IsAlbum(const URI& uri);

	SCDLL bool IsImgurLink(const URI& uri);

	SCDLL bool IsDirect(const URI& uri);

    SCDLL const std::vector<String> GetAlbum(const URI& URL, const String& ClientId);
    SCDLL const String GetImage(const String& ImageHash, const String& ClientId);

}
