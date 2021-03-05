#pragma once

#include "StringOps.hpp"
#include "Link.hpp"
#include "Comment.hpp"
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
	class Video : public Link
	{
	public:
		Video() : HasAudio(false){}
		void GetVideoInfo();
		bool Download(std::filesystem::path destination);
		void Mux(std::filesystem::path destination);

		/**
		* Checks if the provided json has an is_video tag, and if it is in fact: a boolean.
		*/
		static bool IsVideo(const JSON::value& json);

		bool operator==(Video& other);
		bool operator!=(Video& other);
	private:
		std::string DashPlaylistUrl, AudioURL;
		bool HasAudio;

		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& boost::serialization::base_object<Link>(*this);
			ar& AudioURL;
			ar& DashPlaylistUrl;
			ar& HasAudio;
		}
	};
};
