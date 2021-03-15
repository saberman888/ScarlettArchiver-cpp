#pragma once

#include "BaseTypes/Link.hpp"
#include "Comment.hpp"
#include <tinyxml2.h>
#include <utility>
#include <boost/serialization/vector.hpp>


namespace Scarlett::Reddit
{

	using VideoInfo = std::tuple<int, std::string>;

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

		inline const std::vector<VideoInfo> GetVideos()
		{
			return videos;
		}

		const std::string GetAudioURL();
		inline const bool HasAudio()
		{
			return audio.has_value();
		}

		bool operator==(Video& other);
		bool operator!=(Video& other);

	private:
		Video() = default;

		/*
		* Using the information from PushShift, Fetch() downloads the dash manifest or dash file, and 
		* parses it with tinyxml2. Then extracts the BaseURLs and heights of videos, and stores said information into the videos vector
		*/
		void Fetch();

		// Occasionally, a video will not have any audio whatsoever; hence why it's optional
		std::optional<std::string> audio{ std::nullopt };
		std::vector<VideoInfo> videos;

		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& boost::serialization::base_object<Link>(*this);
			ar& audio;
			ar& videos;
		}

		/*
		* Takes video and puts it into a selective position: sorted from SMALL to BIG, or 0 .. 9+
		* This makes it so everytime you get the back, you always get the best resolution immediately.
		*/
		void AddVideo(const VideoInfo video);

		using Link::GetContent;
	};
};
