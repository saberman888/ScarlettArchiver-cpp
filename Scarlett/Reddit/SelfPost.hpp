#pragma once

#include <string>
#include <filesystem>
#include <fstream>
#include "RedditCommon.hpp"
#include <boost/serialization/export.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

namespace ScarlettArchiver::RedditAsset
{
	class SelfPost : public RedditCommon
	{
	public:
		/**
		* An empty constructor because, boost's serialization requires it.
		*/
		SelfPost(){}
		SelfPost(const nlohmann::json& json);

		std::string Text;
		static inline bool IsSelfPost(const nlohmann::json& json)
		{
			return (json.contains("is_self") && json.at("is_self").get<bool>());
		}

	private:
		/**
		* Parses and assigns values from Reddit's json into the class' values
		*/
		void Read(const nlohmann::json& json);
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& boost::serialization::base_object<RedditCommon>(*this);
			ar& Text;
		}
	};
};
