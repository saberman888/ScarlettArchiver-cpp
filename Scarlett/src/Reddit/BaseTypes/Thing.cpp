#include "Scarlett/Reddit/BaseTypes/Thing.hpp"
#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/export.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include "../../Internal/Exceptions.hpp"

BOOST_SERIALIZATION_ASSUME_ABSTRACT(Scarlett::Reddit::Thing)
BOOST_SERIALIZATION_SHARED_PTR(Scarlett::Reddit::Thing)
BOOST_CLASS_EXPORT_GUID(Scarlett::Reddit::Thing, "RedditBase")
namespace Scarlett::Reddit
{

	template<class Archive>
	void Thing::serialize(Archive& ar, const unsigned int version)
	{
		using namespace boost::serialization;
		ar& make_nvp("name", Name);
		ar& make_nvp("id", Id);
		ar& make_nvp("author", Author);
		ar& make_nvp("permalink", Permalink);
		ar& make_nvp("created_local", CreatedLocal);
		ar& make_nvp("created_utc", CreatedUTC);
	}

	template void Thing::serialize<boost::archive::xml_oarchive>(boost::archive::xml_oarchive& ar, const unsigned int version);
	template void Thing::serialize<boost::archive::xml_iarchive>(boost::archive::xml_iarchive& ar, const unsigned int version);

	Thing::Thing() = default;
	Thing::Thing(const JsonValue& json) { Read(json); }

	const Thing::Timestamp Thing::getCreatedLocalTime() { return CreatedLocal; }
	const Thing::Timestamp Thing::getCreatedUTCTime() { return CreatedUTC; }
	const String Thing::getAuthor() { return Author; }
	const String Thing::getId() { return Id; }
	const String Thing::getName() { return Name; }
	const String Thing::getPermalink() { return Permalink; }

	bool Thing::operator==(const Thing& thing)
	{
		return (Name == thing.Name) && 
			(Id == thing.Id) && 
			(Author == thing.Author) && 
			(Permalink == thing.Permalink) && 
			(CreatedUTC == thing.CreatedUTC) && 
			(CreatedLocal == thing.CreatedLocal);
	}

	bool Thing::operator!=(const Thing& thing)
	{
		return !this->operator==(thing);
	}

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
