#include "Scarlett/Reddit/BaseTypes/Link.hpp"
BOOST_CLASS_EXPORT(Scarlett::Reddit::Link)
BOOST_SERIALIZATION_SHARED_PTR(Scarlett::Reddit::Link)


namespace Scarlett::Reddit
{
	Link::Link(const JsonValue& json, std::optional<String> ImgurClientId) : ImgurClientId(ImgurClientId), Thing(json)
	{}

	String Link::GetContent() {
		using namespace Scarlett::Media;

		log->info("{}: Getting Image", toString(Id));
		if (ImgurClientId) {
			log->info("{}: Resolving the URL through the Imgur API", toString(Id));
			return ImgurAccess::GetImage(URL.GetURLString(),
				ImgurClientId.value());
		}
		else {
			log->info("{]: Image returned", toString(Id));
			return URL.GetURLString();
		}
	}

	bool Link::operator==(Link& other)
	{
		return Thing::operator==(other);
	}

	bool Link::operator!=(Link& other)
	{
		return Thing::operator!=(other);
	}

	void Link::Read(const JsonValue& json)
	{
		try {
			if (json.has_string_field("post_hint"_u)) {
				Hint = json.at("post_hint"_u).as_string();
			}
		}
		catch (const JSON::json_exception& e) {
			scarlettNestedThrow(e.what());
		}
	}


};
