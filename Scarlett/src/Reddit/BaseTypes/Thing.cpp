#include "Scarlett/Reddit/BaseTypes/Thing.hpp"
BOOST_SERIALIZATION_ASSUME_ABSTRACT(Scarlett::Reddit::Thing)
BOOST_SERIALIZATION_SHARED_PTR(Scarlett::Reddit::Thing)
BOOST_CLASS_EXPORT_IMPLEMENT(Scarlett::Reddit::Thing)
namespace Scarlett::Reddit
{
	void Thing::Read(const JsonValue& value)
	{
		try {
			Name = value.at("name"_u).as_string();
			Id = value.at("id"_u).as_string();
			Permalink = value.at("permalink"_u).as_string();
			Author = value.at("author"_u).as_string();
			CreatedUTC = static_cast<time_t>(value.at("created_utc"_u).as_integer());
			CreatedLocal = static_cast<time_t>(value.at("created"_u).as_integer());
		}
		catch (web::json::json_exception& e) {
			scarlettNestedThrow(e.what());
		}
	}
};
