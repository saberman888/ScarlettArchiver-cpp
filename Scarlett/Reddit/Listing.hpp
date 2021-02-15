#pragma once

#include "RedditCommon.hpp"
#include <vector>
#include <string>
#include <memory>

namespace ScarlettArchiver
{
	template<class T>
	class Listing
	{
	public:
		std::vector<std::unique_ptr<T>> items;
		virtual nlohmann::json Next() { return nlohmann::json(); }
		virtual void Read(const nlohmann::json& json){}
		virtual void Write(){}
		virtual size_t size() { return items.size(); }
	};
};
