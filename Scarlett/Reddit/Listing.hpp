#pragma once

#include "RedditCommon.hpp"
#include <vector>
#include <string>
#include <memory>

namespace ScarlettArchiver
{
	class Listing
	{
	public:
		std::vector<std::unique_ptr<RedditAsset::RedditCommon>> items;
		virtual nlohmann::json Next() { return nlohmann::json(); }
		virtual void Read(const nlohmann::json& json){}
		virtual void Write(){}
	};
};
