#include "Comment.hpp"

namespace ScarlettArchiver::RedditAsset
{
	Comment::Comment(const nlohmann::json& json) : Text("[deleted]") {
		RedditCommon::initLog();
		Comment::Read(json);
	}

	void Comment::Read(const nlohmann::json& json) {
		try {
			RedditCommon::Read(json);
			if(json.contains("body"))
				Text = json.at("body").get<std::string>();
		}
		catch (nlohmann::json::exception& e) {
			std::throw_with_nested(ScarlettArchiver::ScarlettPostException("Failed to extract JSON", this->Id));
		}
	}

};
