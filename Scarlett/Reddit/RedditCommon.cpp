#include "RedditCommon.hpp"

namespace ScarlettArchiver::RedditAsset
{
	bool RedditCommon::operator==(const RedditCommon& other)
	{
		return (URL == other.URL && Id == other.Id && Author == other.Author && Permalink == other.Permalink && Title == other.Title && Domain == other.Domain);
	}
	bool RedditCommon::operator!=(const RedditCommon& other)
	{
		return !this->operator==(other);
	}
	void RedditCommon::initLog()
	{
		log = spdlog::get("Scarlett Archiver Log");
	}
	void RedditCommon::Read(const nlohmann::json& json)
	{
		try {

			Id = json.at("id").get<std::string>();
			log->info("ID: " + Id);


			Author = json.at("author").get<std::string>();

			Permalink = json.at("permalink").get<std::string>();
			log->info("Permalink: " + Permalink);

			if(json.contains("title"))
				Title = json.at("title").get<std::string>(); 

			CreatedUTC = json.at("created_utc").get<time_t>();

			if (json.contains("link_url"))
			{
				URL = json.at("link_url").get<std::string>();
			}
			else if(json.contains("url")){
				URL = json.at("url").get<std::string>();
			}
			
			if (json.contains("domain") && json.at("domain").is_string())
				Domain = json.at("domain").get<std::string>();
		}
		catch (nlohmann::json::exception& e) {
			scarlettNestedThrow("Failed to parse JSON for RedditCommon, " + std::string(e.what()));

		}
	}

	void RedditCommon::SerializeTo(SerializationMethod sm)
	{
		SerializeAs = sm;
	}
}
