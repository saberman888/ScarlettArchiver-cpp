#pragma once

#include <string>
#include <filesystem>
#include <fstream>
#include "RedditCommon.hpp"
#include "BasicRequest.hpp"

namespace ScarlettArchiver::RedditAsset
{
	class Comment : public RedditCommon
	{
	public:
		Comment(const nlohmann::json& json);
		std::string Text;
	private:
		void Read(const nlohmann::json& json);
	};
};
