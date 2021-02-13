#include "Reddit/CommentListing.hpp"

namespace ScarlettArchiver::RedditAsset
{
    CommentListing::CommentListing(const std::string ParentId) : ParentId(ParentId)
    {
    }

    void CommentListing::Read(const nlohmann::json& json)
    {
        for (auto com : json.at(1).at("data").at("children"))
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
                auto json = nlohmann::json::parse(data.buffer);
                return json;
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