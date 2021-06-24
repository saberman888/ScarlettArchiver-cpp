#include "Scarlett/Reddit/BaseTypes/Linkable.hpp"
BOOST_SERIALIZATION_ASSUME_ABSTRACT(Scarlett::Reddit::BaseTypes::Linkable);
BOOST_SERIALIZATION_SHARED_PTR(Scarlett::Reddit::BaseTypes:::Linkable);

namespace Scarlett::Reddit::BaseTypes
{
	bool Linkable::operator==(Linkable& other)
	{
		return ((Domain == other.Domain) && (Title == other.Title) && (URL.GetURLString() == other.URL.GetURLString()));
	}
	bool Linkable::operator!=(Linkable& other)
	{
		return ((Domain != other.Domain) && (Title != other.Title) && (URL.GetURLString() != other.URL.GetURLString()));
	}
	void Linkable::Read(const JSON::value& json)
	{
		try {
			Domain = json.at("domain"_u).as_string();
			Title = json.at("title"_u).as_string();
			if (json.has_string_field("url"_u))
			{
				URL = json.at("url"_u).as_string();
			}
			else {
				URL = json.at("link_url"_u).as_string();
			}
		}
		catch (const JSON::json_exception& e) {
			scarlettNestedThrow(e.what());
		}
	}
}