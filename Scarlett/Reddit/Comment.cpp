#include "Comment.hpp"

namespace ScarlettArchiver::RedditAsset
{
	Comment::Comment(const nlohmann::json& json) : Text("[deleted]") {
		RedditCommon::initLog();
		Comment::Read(json);
		replies = std::make_unique<CommentListing>(Id);
	}
	

	bool Comment::operator==(const Comment& other)
	{
		return (RedditCommon::operator==(other) && other.Text == Text);
	}

	bool Comment::operator!=(const Comment& other)
	{
		return (RedditCommon::operator!=(other) && Text != other.Text);
	}

	void Comment::Read(const nlohmann::json& json) {
		this->Domain = "self.";
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
