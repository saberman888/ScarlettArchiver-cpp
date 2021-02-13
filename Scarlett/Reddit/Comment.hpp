#pragma once

#include <string>
#include <filesystem>
#include <fstream>
#include <memory>
#include <vector>

#include "RedditCommon.hpp"
#include "CommentListing.hpp"
#include "BasicRequest.hpp"

namespace ScarlettArchiver::RedditAsset
{
	class Comment : public RedditCommon
	{
	public:
		Comment(const nlohmann::json& json);
		std::unique_ptr<CommentListing> replies;
		std::string Text;

		bool operator==(const Comment& other);
		bool operator!=(const Comment& other);
	private:
		void Read(const nlohmann::json& json);
	};
};
