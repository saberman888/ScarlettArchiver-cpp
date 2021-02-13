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


    CommentListing::CommentListing(const std::string ParentId) : ParentId(ParentId)
    {
    }

    void CommentListing::Read(const nlohmann::json& json)
    {
        for (auto com : json.at("data").at("children"))
        {
            auto tempComment = std::make_unique<RedditAsset::Comment>(com.at("data"));
            items.push_back(std::move(tempComment));
            if (!com.at("data").at("replies").at("data").at("children").empty())
                tempComment->replies->Read(com.at("data").at("replies"));
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
                throw;
            }
        }
        else {
            throw ScarlettArchiver::ScarlettException("Failed to get comment data from " + ParentId);
        }
        return nlohmann::json();

    }
};
