#include "Galleries.hpp"
BOOST_CLASS_EXPORT(Scarlett::Reddit::Gallery);
BOOST_SERIALIZATION_SHARED_PTR(ScarletT::Reddit::Gallery);


namespace Scarlett::Reddit
{
	Gallery::Gallery(const JSON::value& json, const std::optional<std::string> ImgurClientId) : Link(json, ImgurClientId)	{
		Read(json);	

		// Since we have all the data, resolve the album if it is an imgur one
		if (Media::ImgurAccess::IsImgurLink(URL.GetURLString()) && ImgurClientId)
		{
			for (auto& urls : Media::ImgurAccess::GetAlbum(URL.GetURLString(), ImgurClientId.value()))
			{
				Images.push_back(urls);
			}
		}
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
					Images.push_back(Media::Content(imageURL));
				}
				catch (JSON::json_exception& e) {
					scarlettNestedThrow("Failed to parse JSON for Gallery, " + std::string(e.what()));
				}
			}
		}
	}

	bool Gallery::IsGallery(const JSON::value& json)
	{
		if ((json.has_boolean_field("is_gallery"_u) && json.at("is_gallery"_u).as_bool() && json.has_field("gallery_data"_u)) ||
			(Media::ImgurAccess::IsAlbum(u8(json.at("url"_u).as_string()))))
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


