#include "Link.hpp"
BOOST_CLASS_EXPORT(ScarlettArchiver::RedditAsset::Link);


namespace ScarlettArchiver::RedditAsset
{
	Link::Link(const JSON::value& json, std::optional<std::string> ImgurClientId) : ImgurClientId(ImgurClientId)
	{
		Linkable::Read(json);
		Postable::Read(json);
	}

	std::string Link::GetContent() {
		log->info(Id + ": Getting Image");
		if (ImgurClientId != std::nullopt) {
			log->info(Id + ": Resolving the URL through the Imgur API");
			return ImgurAccess::GetImage(URL,
				ImgurClientId.value());
		}
		else {
			log->info(Id + ": Image returned");
			return URL;
		}
	}

	bool Link::operator==(Link& other)
	{
		return Linkable::operator==(other) && Postable::operator==(other);
	}

	bool Link::operator!=(Link& other)
	{
		return Linkable::operator!=(other) && Postable::operator!=(other);
	}

	void Link::Read(const JSON::value& json)
	{
		try {
			Hint = ToU8String(json.at("post_hint"_u).as_string());
		}
		catch (const JSON::json_exception& e) {
			scarlettNestedThrow(e.what());
		}
	}


};
