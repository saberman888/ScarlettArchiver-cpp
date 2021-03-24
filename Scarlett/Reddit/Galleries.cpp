#include "Galleries.hpp"
BOOST_CLASS_EXPORT(Scarlett::Reddit::Gallery);


namespace Scarlett::Reddit
{
	Gallery::Gallery(const JSON::value& json, const std::optional<std::string> ImgurClientId) : Link(json, ImgurClientId)	{
		Read(json);	
	}

	void Gallery::Read(const JSON::value& json)
	{
		if (!ImgurClientId) {
			for (auto& image : json.at("gallery_data"_u).at("items"_u).as_array())
			{
				try {
					auto mediaId = image.at("media_id"_u).as_string();
					auto mediaMetadata = json.at("media_metadata"_u).at(mediaId);

					std::string imageExtension = Scarlett::splitString(
						u8(mediaMetadata.at("m"_u).as_string()), 
						'/')[1];
					std::string imageURL = "https://i.redd.it/" + conv::to_utf8string(mediaId) + "." + imageExtension;
					Images.push_back(imageURL);
				}
				catch (JSON::json_exception& e) {
					scarlettNestedThrow("Failed to parse JSON for Gallery, " + std::string(e.what()));
				}
			}
		}
	}

	const std::vector<std::string> Gallery::GetImages()
	{
        if (!ImgurClientId)
		{
			return Images;
		}
		else {
			return Media::ImgurAccess::ResolveAlbumURLs(URL, ImgurClientId.value());
		}
	}
	bool Gallery::IsGallery(const JSON::value& json)
	{
		if ((json.has_boolean_field("is_gallery"_u) && json.at("is_gallery"_u).as_bool() && json.has_field("gallery_data"_u)) ||
			(u8(json.at("url"_u).as_string()).find("imgur.com/a/") != std::string::npos ))
			return true;
		return false;
	}
	bool Gallery::operator==(Gallery& other)
	{
		return Linkable::operator==(other) && Postable::operator==(other);
	}

	bool Gallery::operator!=(Gallery& other)
	{
		return Linkable::operator!=(other) && Postable::operator==(other);
	}

	
};


