#include "RedditCommon.hpp"

namespace ScarlettArchiver::RedditAsset
{
	bool Postable::operator==(Postable& other)
	{
		return (timediff(mktime(&CreatedUTC), mktime(&other.CreatedUTC)) && (Author == other.Author) && (Id == other.Id) && (Permalink == other.Permalink));
	}

	bool Postable::operator!=(Postable& other)
	{
		return (!timediff(mktime(&CreatedUTC), mktime(&other.CreatedUTC)) && (Author != other.Author) && (Id != other.Id) && (Permalink != other.Permalink));
	}

	void Postable::Read(const nlohmann::json& json)
	{
		try {
			json.at("author").get_to(Author);
			json.at("id").get_to(Id);
			json.at("permalink").get_to(Permalink);
			time_t elemTime = json.at("created_utc").get<time_t>();
			CreatedUTC = *std::gmtime(&elemTime);
		}
		catch (const nlohmann::json::exception& e) {
			scarlettNestedThrow(e.what());
		}
	}
	bool Postable::timediff(unsigned int end, unsigned int begin)
	{
		return std::difftime(
			max(end, begin),
			min(end, begin)
		) == 0;
	}
	bool Linkable::operator==(const Linkable& other)
	{
		return ((Domain == other.Domain) && (Title == other.Title) && (URL == other.URL));
	}
	bool Linkable::operator!=(const Linkable& other)
	{
		return ((Domain != other.Domain) && (Title != other.Title) && (URL != other.URL));
	}
	void Linkable::Read(const nlohmann::json& json)
	{
		try {
			json.at("domain").get_to(Domain);
			json.at("title").get_to(Title);
			if (json.contains("url"))
			{
				json.at("url").get_to(URL);
			}
			else {
				json.at("link_url").get_to(URL);
			}
		}
		catch (const nlohmann::json::exception& e) {
			scarlettNestedThrow(e.what());
		}
	}
}
