#include "Comment.hpp"
//BOOST_CLASS_EXPORT(ScarlettArchiver::RedditAsset::CommentTree);

namespace ScarlettArchiver::RedditAsset
{

    Comment::Comment(const std::string& ParentId) : ParentId(ParentId)
    {
    }
	Comment::Comment(const nlohmann::json& json, std::optional<std::string> ParentId) : ParentId(ParentId)
	{
		TextPost::Read(json);
	}
    void Comment::GetRedditComments()
    {
		auto data = Download("https://reddit.com/" + ParentId.value() + ".json");
		if (data.AllGood())
		{
			try {
				nlohmann::json json = nlohmann::json::parse(data.buffer);
				Read(json.at(1));
			}
			catch (nlohmann::json::exception& e) {
				scarlettNestedThrow("Failed to parse JSON for Comment, " + std::string(e.what()));
			}
		}
		else {
			std::string msg = "Failed to download additional comments for a post, " + std::to_string(data.HttpState) + ", " + data.Message;
			scarlettThrow(msg);
		}
    }

	void Comment::GetPushShiftComments()
	{
		// TODO: Implementing getting the comments from pushsshift
	}

	void Comment::Read(const nlohmann::json& json)
	{
		ScarlettArchiver::Write(json, std::filesystem::path("logs"), "elem.json");
		for (auto elem : json.at("data").at("children"))
		{
			ScarlettArchiver::Write(elem.at("data"), "logs", "innerelement.json");
			try {

				auto com = elem.at("data");
				// If it has an array called children, it's probably a 'more children' object, or in other words keys to more comments
				if (com.contains("children") && com.at("children").is_array())
				{
					// TODO: Implement Reddit API components to use more children object
					continue;
				}

				auto tempComment = std::make_unique<Comment>(com, ParentId);

				if (com.at("replies").is_object() && com.at("replies").is_structured()) {
					auto innerchildren = com.at("replies").at("data").at("children");
				}

				replies.push_back(std::move(tempComment));
			}
			catch (nlohmann::json::exception& e) {
				scarlettNestedThrow("Failed to parse comment JSON from CommentListing, " + ParentId.value() + ", " + std::string(e.what()));
			}
		}
	}
};
