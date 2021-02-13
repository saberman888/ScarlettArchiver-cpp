#pragma once

#include <string>
#include <vector>
#include <memory>

#include "nlohmann/json.hpp"
#include "Listing.hpp"
#include "Comment.hpp"
#include "Misc.hpp"
#include "exceptions.hpp"

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
}