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
		try {
			RedditCommon::Read(json);
			if(json.contains("body"))
				Text = json.at("body").get<std::string>();
		}
		catch (nlohmann::json::exception& e) {
			scarlettNestedThrow("Failed to extract JSON for Comment, " + std::string(e.what()));
		}
	}


    CommentListing::CommentListing(const std::string ParentId) : ParentId(ParentId)
    {
    }

    void CommentListing::Read(const nlohmann::json& json)
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
                
                auto tempComment = std::make_unique<RedditAsset::Comment>(com);
                
                if (com.at("replies").is_object() && com.at("replies").is_structured()) {
                    auto innerchildren = com.at("replies").at("data").at("children");
                    tempComment->replies->Read(com.at("replies"));
                }

                items.push_back(std::move(tempComment));
            }
            catch (nlohmann::json::exception& e) {
                scarlettNestedThrow("Failed to parse comment JSON from CommentListing, " + ParentId + ", " + std::string(e.what()));
            }
        }
    }

    nlohmann::json CommentListing::Next()
    {
        auto data = Download("https://reddit.com/" + ParentId + ".json");
        if (data.AllGood())
        {
            try {
                nlohmann::json json = nlohmann::json::parse(data.buffer);
                return json.at(1);
            }
            catch (nlohmann::json::exception& e) {
                scarlettNestedThrow("Failed to parse JSON for Comment, " + std::string(e.what()));
            }
        }
        else {
            std::string msg = "Failed to download additional comments for a post, " + std::to_string(data.HttpState) + ", " + data.Message;
            scarlettThrow(msg);
        }
        return nlohmann::json();

    }
};
