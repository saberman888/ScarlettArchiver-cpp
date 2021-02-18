#include "Galleries.hpp"
BOOST_CLASS_EXPORT(ScarlettArchiver::RedditAsset::Gallery);


namespace ScarlettArchiver::RedditAsset
{
	Gallery::Gallery(const nlohmann::json& json, const std::optional<std::string> ImgurClientId) : Link(json, ImgurClientId)	{
		Read(json);	
	}

	void Gallery::Read(const nlohmann::json& json)
	{
		if (ImgurClientId == std::nullopt) {
			for (auto& image : json.at("gallery_data").at("items"))
			{
				try {
					std::string mediaId = image.at("media_id").get<std::string>();
					auto mediaMetadata = json.at("media_metadata").at(mediaId);

					std::string imageExtension = ScarlettArchiver::splitString(mediaMetadata.at("m").get<std::string>(), '/')[1];
					std::string imageURL = "https://i.redd.it/" + mediaId + "." + imageExtension;
					Images.push_back(imageURL);
				}
				catch (nlohmann::json::exception& e) {
					scarlettNestedThrow("Failed to parse JSON for Gallery, " + std::string(e.what()));
				}
			}
		}
	}

	const std::vector<std::string> Gallery::GetImages()
	{
        if (ImgurClientId == std::nullopt)
		{
			return Images;
		}
		else {
			return ImgurAccess::ResolveAlbumURLs(URL, ImgurClientId.value());
		}
	}
	bool Gallery::IsGallery(const nlohmann::json& json)
	{
		if (json.contains("is_gallery") && json.at("is_gallery").get<bool>() && json.at("gallery_data").is_object())
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


