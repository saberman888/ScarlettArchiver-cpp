#include "Linkable.hpp"
BOOST_SERIALIZATION_ASSUME_ABSTRACT(Scarlett::Reddit::BaseTypes::Linkable);

namespace Scarlett::Reddit::BaseTypes
{
	bool Linkable::operator==(Linkable& other)
	{
		return ((Domain == other.Domain) && (Title == other.Title) && (URL.GetURL() == other.URL.GetURL()));
	}
	bool Linkable::operator!=(Linkable& other)
	{
		return ((Domain != other.Domain) && (Title != other.Title) && (URL.GetURL() != other.URL.GetURL()));
	}
	void Linkable::Read(const JSON::value& json)
	{
		try {
			Domain = u8(json.at("domain"_u).as_string());
			Title = u8(json.at("title"_u).as_string());
			if (json.has_string_field("url"_u))
			{
				URL = u8(json.at("url"_u).as_string());
			}
			else {
				URL = u8(json.at("link_url"_u).as_string());
			}
		}
		catch (const JSON::json_exception& e) {
			scarlettNestedThrow(e.what());
		}
	}
}