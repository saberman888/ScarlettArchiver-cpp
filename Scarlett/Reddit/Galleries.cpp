#include "Galleries.hpp"
BOOST_CLASS_EXPORT(ScarlettArchiver::RedditAsset::Gallery);


namespace ScarlettArchiver::RedditAsset
{
	Gallery::Gallery(const nlohmann::json& json)
	{
		Read(json);
	}

	Gallery::Gallery(const nlohmann::json& json, std::string ImgurClientId) : ImgurClientId(ImgurClientId)
	{
		Read(json);
	}

	void Gallery::Read(const nlohmann::json& json)
	{
		RedditCommon::Read(json);
		if (ImgurClientId.empty()) {
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
					std::throw_with_nested(ScarlettArchiver::ScarlettPostException("Failed to extract JSON", this->Id));
				}
			}
		}
	}

	const std::vector<std::string> Gallery::GetImages()
	{
        if (ImgurClientId.empty())
		{
			return Images;
		}
		else {
			return ImgurAccess::ResolveAlbumURLs(URL.value(), ImgurClientId);
		}
	}
	bool Gallery::IsGallery(const nlohmann::json& json)
	{
		if (json.contains("is_gallery") && json.at("is_gallery").get<bool>() && json.at("gallery_data").is_object())
			return true;
		return false;
	}
	bool Gallery::operator==(const Gallery& other)
	{
		return RedditCommon::operator==(other);
	}

	bool Gallery::operator!=(const Gallery& other)
	{
		return RedditCommon::operator!=(other);
	}

	
};


