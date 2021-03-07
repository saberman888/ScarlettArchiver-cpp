#include "Link.hpp"
BOOST_CLASS_EXPORT(Scarlett::RedditAsset::BaseTypes::Link)


namespace Scarlett::RedditAsset::BaseTypes
{
	Link::Link(const JSON::value& json, std::optional<std::string> ImgurClientId) : ImgurClientId(ImgurClientId)
	{
		Linkable::Read(json);
		Postable::Read(json);
	}

	std::string Link::GetContent() {
		using namespace Scarlett::Media;

		log->info(Id + ": Getting Image");
		if (ImgurClientId) {
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
