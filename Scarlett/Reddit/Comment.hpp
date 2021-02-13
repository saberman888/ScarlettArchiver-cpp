#pragma once

#include <string>
#include <filesystem>
#include <fstream>
#include <memory>
#include <vector>

#include "RedditCommon.hpp"
#include "BasicRequest.hpp"
#include "Listing.hpp"
#include "Misc.hpp"

namespace ScarlettArchiver::RedditAsset
{
	class CommentListing : public Listing {
	public:
		CommentListing(const std::string ParentId);

		nlohmann::json Next();
		void Read(const nlohmann::json& json);
	private:
		std::string ParentId;
	}; 
	
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
