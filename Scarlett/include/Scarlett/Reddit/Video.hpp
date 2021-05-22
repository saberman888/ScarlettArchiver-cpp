#pragma once

#include "BaseTypes/Link.hpp"
#include "Comment.hpp"
#include <tinyxml2.h>
#include <utility>
#include <boost/serialization/vector.hpp>


namespace Scarlett::Reddit
{
	struct VideoInfo
	{
		int Height;
		std::string BaseURL;

		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			using namespace boost::serialization;
			ar& make_nvp("Height", Height);
			ar& make_nvp("BaseURL", BaseURL);
		}
	};

	class Video : public BaseTypes::Link
	{
	public:
		Video(const JSON::value& json);
		
		/**
		* Checks if the provided json has an is_video tag, and if it is in fact: a boolean.
		* It checks for the values: is_value and post_hint.
		*/
		static bool IsVideo(const JSON::value& json);

		/*
			Glue video and audio together into one file using ffmpeg with Mux
		*/
		void Mux(std::filesystem::path destination);
		
		inline const size_t TotalVideos()
		{
			return videos.size();
		}
		/*
		* Returns a list of the same video, but in different qualities.
		* Returns a vector of structs consisting of: 
		*	int Height
		*	std::string BaseURL
		*/
		inline const std::vector<VideoInfo> GetVideos()
		{
			return videos;
		}

		/*
		*	Returns a full URL of the video's audio, if it's there.
		*/
		const std::string GetAudioURL();
		inline const bool HasAudio()
		{
			return audio.has_value();
		}

		bool operator==(Video& other);
		bool operator!=(Video& other);
		std::string GetContent() = delete;

	private:
		Video() {};

		/*
		* Using the information from PushShift, Fetch() downloads the dash manifest or dash file, and 
		* parses it with tinyxml2. Then extracts the BaseURLs and heights of videos, and stores said information into the videos vector
		*/
		void Fetch();

		// Occasionally, a video will not have any audio whatsoever; hence why it's optional
		boost::optional<std::string> audio{ boost::none };
		std::vector<VideoInfo> videos;

		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			using namespace boost::serialization;
			ar& make_nvp("Link", base_object<Link>(*this));
			ar& make_nvp("Audio", audio);
			ar& make_nvp("VideoInformation", videos);
		}

		/*
		* Takes video and puts it into a selective position: sorted from SMALL to BIG, or 0 .. 9+
		* This makes it so everytime you get the back, you always get the best resolution immediately.
		*/
		void AddVideo(const VideoInfo video);
	};
};