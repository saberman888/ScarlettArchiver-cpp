#include "RedditCommon.hpp"

namespace ScarlettArchiver::RedditAsset
{
	void RedditCommon::initLog()
	{
		log = spdlog::get("Scarlett Archiver Log");
	}
	void RedditCommon::Read(const nlohmann::json& json)
	{
		try {
			if (json.contains("link_url"))
			{
				URL = json.at("link_url").get<std::string>();
			}
			else {
				URL = json.at("url").get<std::string>();
			}

			Id = json.at("id").get<std::string>();
			log->info("ID: " + Id);


			Author = json.at("author").get<std::string>();

			Permalink = json.at("permalink").get<std::string>();
			log->info("Permalink: " + Permalink);

			Title = json.at("title").get<std::string>();
			if (json.contains("domain"))
			{
				if (json.at("domain").is_null())
				{	
					Domain = "self.reddit";
				}
				else {
					Domain = json.at("domain").get<std::string>();
				}

			}
			else {
				log->info("No domain found for element: " + Id);
				log->warn("Setting domain to self.reddit");
				Domain = "self.reddit";
			}
			CreatedUTC = json.at("created_utc").get<time_t>();
		}
		catch (nlohmann::json::exception& e) {
			log->error("Exception triggered: nlohmann::json");
			log->error(e.what());
			log->info("Rethrowing...");
			std::throw_with_nested(ScarlettArchiver::ScarlettPostException("Failed to extract JSON", this->Id));

		}
	}

	void RedditCommon::SerializeTo(SerializationMethod sm)
	{
		SerializeAs = sm;
	}
}
