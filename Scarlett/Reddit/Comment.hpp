#pragma once

#include <string>
#include <memory>
#include <vector>
#include <optional>

#include "BasicRequest.hpp"
#include "Misc.hpp"
#include "TextPost.hpp"
#include "exceptions.hpp"

namespace ScarlettArchiver::RedditAsset
{
	
	class Comment : public TextPost
	{
	public:
		Comment(const std::string& ParentId);
		Comment(const nlohmann::json& json, std::optional<std::string> ParentId = std::nullopt);
		std::vector<std::unique_ptr<Comment>> replies; 
		void GetRedditComments();
		void GetPushShiftComments();

	private:
		std::optional<std::string> ParentId;
		void Read(const nlohmann::json& json);
	};	
	using CommentTree = std::vector<std::unique_ptr<Comment>>;
};
