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

	void Postable::Read(const nlohmann::json& json)
	{
		try {
			json.at("author").get_to(Author);
			json.at("id").get_to(Id);
			json.at("permalink").get_to(Permalink);
			json.at("creatd_utc").get_to(CreatedUTC);
		}
		catch (const nlohmann::json::exception& e) {
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
