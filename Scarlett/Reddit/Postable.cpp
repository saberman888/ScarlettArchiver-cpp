#include "Postable.hpp"
BOOST_SERIALIZATION_ASSUME_ABSTRACT(ScarlettArchiver::RedditAsset::Postable);

namespace ScarlettArchiver::RedditAsset
{
	bool Postable::operator==(Postable& other)
	{
		return (timediff(CreatedUTC, other.CreatedUTC) && (Author == other.Author) && (Id == other.Id) && (Permalink == other.Permalink));
	}

	bool Postable::operator!=(Postable& other)
	{
		return (!timediff(CreatedUTC, other.CreatedUTC) && (Author != other.Author) && (Id != other.Id) && (Permalink != other.Permalink));
	}

	void Postable::Read(const JSON::value& json)
	{
		try {
			Author = ToU8String(json.at("author"_u).as_string());
			Id = ToU8String(json.at("Id"_u).as_string());
			Permalink = ToU8String(json.at("permalink"_u).as_string());
			CreatedUTC = static_cast<time_t>(json.at("created_utc"_u).as_integer());
		}
		catch (const JSON::json_exception& e) {
			scarlettNestedThrow(e.what());
		}
	}
	bool Postable::timediff(time_t end, time_t begin)
	{
		return std::difftime(
			( end > begin ? end : begin),
			(end < begin ? end : begin)
		) == 0;
	}
};
