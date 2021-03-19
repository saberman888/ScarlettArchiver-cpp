#include "Comment.hpp"
BOOST_CLASS_EXPORT(Scarlett::Reddit::Comment)

namespace Scarlett::Reddit
{

    Comment::Comment(const std::string& ParentId)
    {
		this->ParentId.emplace(ParentId);
    }
	Comment::Comment(const JSON::value& json, boost::optional<std::string> ParentId)
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
			std::string msg = "Failed to gather additional comments!, " + std::to_string(data.status_code());
			scarlettThrow(msg);
		}
    }


	void Comment::Read(const JSON::value& json)
	{
		Scarlett::Write(json, std::filesystem::path("logs"), "elem.json");
		for (auto elem : json.at("data"_u).at("children"_u).as_array())
		{
			Scarlett::Write(elem.at("data"_u), "logs", "innerelement.json");
			try {

				auto com = elem.at("data"_u);
				// If it has an array called children, it's probably a 'more children' object, or in other words keys to more comments
				if (com.has_field("children"_u) && com.at("children"_u).is_array())
				{
					// TODO: Implement Reddit API components to use more children object
					continue;
				}

				auto tempComment = std::make_unique<Comment>(com, ParentId);

				if (auto replies = com.at("replies"_u); replies.is_array() && replies.as_array().size() > 0) {
					auto innerchildren = com.at("replies"_u).at("data"_u).at("children"_u);
				}

				replies.push_back(std::move(tempComment));
			}
			catch (JSON::json_exception& e) {
				scarlettNestedThrow("Failed to parse comment JSON from CommentListing, " + ParentId.value() + ", " + std::string(e.what()));
			}
		}
	}
};
