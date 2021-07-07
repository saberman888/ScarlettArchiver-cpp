#pragma once

#include "../Internal/Types.hpp"
namespace Scarlett::Media::ImgurAccess
{
	SCDLL bool IsAlbum(const URI& uri);

	SCDLL bool IsImgurLink(const URI& uri);

	SCDLL bool IsDirect(const URI& uri);

    SCDLL const std::vector<String> GetAlbum(const URI& URL, const String& ClientId);
    SCDLL const String GetImage(const String& ImageHash, const String& ClientId);

}
