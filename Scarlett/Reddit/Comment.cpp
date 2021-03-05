#include "Comment.hpp"
BOOST_CLASS_EXPORT(ScarlettArchiver::RedditAsset::Comment);

namespace ScarlettArchiver::RedditAsset
{

    Comment::Comment(const std::string& ParentId) : ParentId(ParentId)
    {
    }
	Comment::Comment(const JSON::value& json, std::optional<std::string> ParentId) : ParentId(ParentId)
	{
		TextPost::Read(json);
	}
    void Comment::GetRedditComments()
    {
		auto data = Download("https://reddit.com/" + ParentId.value() + ".json");
		if (data.status_code() == 200)
		{
			try {
				Read(data.extract_json().get().at(1));
			}
			catch (JSON::json_exception& e) {
				scarlettNestedThrow("Failed to parse JSON for Comment, " + std::string(e.what()));
			}
		}
		else {
			std::string msg = "Failed to download additional comments for a post, " + std::to_string(data.status_code());
			scarlettThrow(msg);
		}
    }

	void Comment::GetPushShiftComments()
	{
		// TODO: Implementing getting the comments from pushsshift
	}

	void Comment::Read(const JSON::value& json)
	{
		ScarlettArchiver::Write(json, std::filesystem::path("logs"), "elem.json");
		for (auto elem : json.at(L"data").at(L"children").as_array())
		{
			ScarlettArchiver::Write(elem.at(L"data"), "logs", "innerelement.json");
			try {

				auto com = elem.at(L"data");
				// If it has an array called children, it's probably a 'more children' object, or in other words keys to more comments
				if (com.has_field(L"children") && com.at(L"children").is_array())
				{
					// TODO: Implement Reddit API components to use more children object
					continue;
				}

				auto tempComment = std::make_unique<Comment>(com, ParentId);

				if (auto replies = com.at(L"replies"); replies.is_array() && replies.as_array().size() > 0) {
					auto innerchildren = com.at(L"replies").at(L"data").at(L"children");
				}

				replies.push_back(std::move(tempComment));
			}
			catch (JSON::json_exception& e) {
				scarlettNestedThrow("Failed to parse comment JSON from CommentListing, " + ParentId.value() + ", " + std::string(e.what()));
			}
		}
	}
};
