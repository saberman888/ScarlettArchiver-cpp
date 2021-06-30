#include "Scarlett/Reddit/Comment.hpp"
BOOST_CLASS_EXPORT(Scarlett::Reddit::Comment)
BOOST_SERIALIZATION_SHARED_PTR(Scarlett::Reddit::Comment)

namespace Scarlett::Reddit
{

    Comment::Comment(const String& ParentId)
    {
		this->ParentId.emplace(ParentId);
    }
	Comment::Comment(const JSON::value& json, boost::optional<String> ParentId)
	{
		TextPost::Read(json);
	}

    void Comment::GetRedditComments()
    {
		auto data = Download("https://reddit.com/"_u + ParentId.value() + ".json"_u);
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
			scarlettThrow("Failed to gather additional comments!, " + std::to_string(data.status_code()));
		}
    }


	void Comment::Read(const JSON::value& json)
	{
		for (auto elem : json.at("data"_u).at("children"_u).as_array())
		{
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
				scarlettNestedThrow("Failed to parse comment JSON from CommentListing, " + toString(ParentId.value_or("???"_u)) + ", " + std::string(e.what()));
			}
		}
	}
};
