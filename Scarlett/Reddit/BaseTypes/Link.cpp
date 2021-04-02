#include "Link.hpp"
BOOST_CLASS_EXPORT(Scarlett::Reddit::BaseTypes::Link)


namespace Scarlett::Reddit::BaseTypes
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
			return ImgurAccess::GetImage(URL.GetURL(),
				ImgurClientId.value());
		}
		else {
			log->info(Id + ": Image returned");
			return URL.GetURL();
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
			if (json.has_string_field("post_hint"_u)) {
				Hint = u8(json.at("post_hint"_u).as_string());
			}
		}
		catch (const JSON::json_exception& e) {
			auto msg = e.what();
			scarlettNestedThrow(msg);
		}
	}


};
