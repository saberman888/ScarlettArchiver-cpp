#include "Postable.hpp"

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
			Author = ToU8String(json.at(L"author").as_string());
			Id = ToU8String(json.at(L"Id").as_string());
			Permalink = ToU8String(json.at(L"permalink").as_string());
			CreatedUTC = static_cast<time_t>(json.at(L"created_utc").as_integer());
		}
		catch (const JSON::json_exception& e) {
			scarlettNestedThrow(e.what());
		}
	}
	bool Postable::timediff(time_t end, time_t begin)
	{
		return std::difftime(
			//max(end, begin),
			( end > begin ? end : begin),
			(end < begin ? end : begin)
		) == 0;
	}
};
