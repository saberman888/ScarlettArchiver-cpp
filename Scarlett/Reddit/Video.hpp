#pragma once

#include "nlohmann/json.hpp"
#include "StringOps.hpp"
#include "RedditCommon.hpp"
#include "Misc.hpp"

#include <string>
#include <filesystem>
#include <exception>
#include <regex>
#include <iosfwd>
#include <boost/serialization/export.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

namespace ScarlettArchiver::RedditAsset
{
	class Video : public RedditCommon
	{
	public:
		Video(){}
		Video(const nlohmann::json& json);
		void GetVideoInfo();
		bool Download(std::filesystem::path destination);
		void Mux(std::filesystem::path destination);

		/**
		* Checks if the provided json has an is_video tag, and if it is in fact: a boolean.
		*/
		static bool IsVideo(const nlohmann::json& json); 
	private:
		std::string DashPlaylistUrl, AudioURL;
		bool HasAudio;

		bool CheckForAudio();
		void Read(const nlohmann::json& json, bool ReadDomain = true); 
		
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& boost::serialization::base_object<RedditCommon>(*this);
			ar& AudioURL;
			ar& DashPlaylistUrl;
			ar& HasAudio;
		}
	};
};
