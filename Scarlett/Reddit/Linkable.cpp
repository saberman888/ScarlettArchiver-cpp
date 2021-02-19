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