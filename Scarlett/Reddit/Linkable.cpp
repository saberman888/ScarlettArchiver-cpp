#include "Linkable.hpp"

namespace ScarlettArchiver::RedditAsset
{
	bool Linkable::operator==(const Linkable& other)
	{
		return ((Domain == other.Domain) && (Title == other.Title) && (URL == other.URL));
	}
	bool Linkable::operator!=(const Linkable& other)
	{
		return ((Domain != other.Domain) && (Title != other.Title) && (URL != other.URL));
	}
	void Linkable::Read(const JSON::value& json)
	{
		try {
			Domain = ToU8String(json.at("domain"_u).as_string());
			Title = ToU8String(json.at("title"_u).as_string());
			if (json.has_string_field("url"_u))
			{
				URL = ToU8String(json.at("url"_u).as_string());
			}
			else {
				URL = ToU8String(json.at("link_url"_u).as_string());
			}
		}
		catch (const JSON::json_exception& e) {
			scarlettNestedThrow(e.what());
		}
	}
}