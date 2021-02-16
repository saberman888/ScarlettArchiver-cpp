#include "Image.hpp"
BOOST_CLASS_EXPORT(ScarlettArchiver::RedditAsset::Link);


namespace ScarlettArchiver::RedditAsset
{
	Link::Link(const nlohmann::json& json, SerializationMethod sm)
	{
		RedditCommon::initLog();
		RedditCommon::Read(json);
		RedditCommon::SerializeTo(sm);
		replies = std::make_shared<CommentListing>(this->Id);
	}

	Link::Link(const nlohmann::json& json, std::string
		ImgurClientId, SerializationMethod sm) : ImgurClientId(ImgurClientId)
	{
		RedditCommon::Read(json); 
		RedditCommon::SerializeTo(sm);
		replies = std::make_shared<CommentListing>(this->Id);
	}

	std::string Link::GetImage() {
		log->info(Id + ": Getting Image");
		if (!ImgurClientId.empty()) {
			log->info(Id + ": Resolving the URL through the Imgur API");
			return ImgurAccess::GetImage(URL.value(),
				ImgurClientId);
		}
		else {
			log->info(Id + ": Image returned");
			return *URL;
		}
	}

	bool Link::operator==(const Link& other)
	{
		return RedditCommon::operator==(other);
	}

	bool Link::operator!=(const Link& other)
	{
		return RedditCommon::operator!=(other);
	}


};
