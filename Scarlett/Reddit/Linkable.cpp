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
			Domain = ToU8String(json.at(L"domain").as_string());
			Title = ToU8String(json.at(L"title").as_string());
			if (json.has_string_field(L"url"))
			{
				URL = ToU8String(json.at(L"url").as_string());
			}
			else {
				URL = ToU8String(json.at(L"link_url").as_string());
			}
		}
		catch (const JSON::json_exception& e) {
			scarlettNestedThrow(e.what());
		}
	}
}