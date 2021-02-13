#include "SelfPost.hpp"
BOOST_CLASS_EXPORT(ScarlettArchiver::RedditAsset::SelfPost);


namespace ScarlettArchiver::RedditAsset
{
	SelfPost::SelfPost(const nlohmann::json& json) : Text("[deleted]") {
		RedditCommon::initLog();
		SelfPost::Read(json);
	}

	bool SelfPost::operator==(const SelfPost& other)
	{
		return (RedditCommon::operator==(other) && Text != other.Text);
	}

	bool SelfPost::operator!=(const SelfPost& other)
	{
		return (RedditCommon::operator!=(other) && other.Text != Text);
	}

	void SelfPost::Read(const nlohmann::json& json) {
		try {
			RedditCommon::Read(json);
			if(json.contains("selftext"))
				Text = json.at("selftext").get<std::string>();
		}
		catch (nlohmann::json::exception& e) {
			log->error("Exception triggered: nlohmann::json");
			log->error("Rethrowing..");
			std::throw_with_nested(ScarlettArchiver::ScarlettPostException("Failed to extract JSON", this->Id));
		}
	}

};