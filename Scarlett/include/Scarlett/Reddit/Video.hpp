#pragma once

#include "BaseTypes/Link.hpp"
#include "Comment.hpp"
#include <tinyxml2.h>
#include <utility>


namespace Scarlett::Reddit
{
	struct VideoInfo
	{
		int Height;
		String BaseURL;

		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive& ar, const unsigned int version);

	};

    class SCDLL Video : public Link
	{
	public:
		Video(const JsonValue& json);
		
		/**
		* Checks if the provided json has an is_video tag, and if it is in fact: a boolean.
		* It checks for the values: is_value and post_hint.
		*/
		static bool IsVideo(const JsonValue& json);

		/*
			Glue video and audio together into one file using ffmpeg with Mux
		*/
		void Mux(std::filesystem::path destination);
		
		const size_t TotalVideos();
		/*
		* Returns a list of the same video, but in different qualities.
		* Returns a vector of structs consisting of: 
		*	int Height
		*	std::string BaseURL
		*/
		const std::vector<VideoInfo> GetVideos();


		/*
		*	Returns a full URL of the video's audio, if it's there.
		*/
		const String GetAudioURL();
		const bool HasAudio();

		bool operator==(Video& other);
		bool operator!=(Video& other);
		String GetContent();

	private:
		Video();

		/*
		* Using the information from PushShift, Fetch() downloads the dash manifest or dash file, and 
		* parses it with tinyxml2. Then extracts the BaseURLs and heights of videos, and stores said information into the videos vector
		*/
		void Fetch();

		// Occasionally, a video will not have any audio whatsoever; hence why it's optional
		boost::optional<String> audio{ boost::none };
		std::vector<VideoInfo> videos;

		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive& ar, const unsigned int version);

		/*
		* Takes video and puts it into a selective position: sorted from SMALL to BIG, or 0 .. 9+
		* This makes it so everytime you get the back, you always get the best resolution immediately.
		*/
		void AddVideo(const VideoInfo video);
	};
};
