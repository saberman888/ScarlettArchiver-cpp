#pragma once

#include "BaseTypes/Link.hpp"
#include "Comment.hpp"
#include "Media/VideoInfo.hpp"

#include <string_view>
#include <regex>
#include <tinyxml2.h>
#include <boost/serialization/vector.hpp>


namespace Scarlett::Reddit
{

	/*
		TODO: In the future make it less dependent on getting the JSON directly from Reddit
	*/
	class Video : public BaseTypes::Link
	{
	public:
		Video(const JSON::value& json);

		void Fetch();
		void Mux(std::filesystem::path destination);

		/**
		* Checks if the provided json has an is_video tag, and if it is in fact: a boolean.
		* It checks for the values: is_value and post_hint.
		*/
		static bool IsVideo(const JSON::value& json);

		bool operator==(Video& other);
		bool operator!=(Video& other);

		inline const int getMaxHeight()
		{
			return maxHeight;
		}

		inline const int getMaxWidth()
		{
			return maxWidth;
		}

		inline const int getMaxFPS()
		{
			return maxFPS;
		}

		inline const size_t getTotalVideos()
		{
			return videos.size();
		}

		inline const std::vector<Media::VideoInfo> getVideos()
		{
			return videos;
		}

		inline const std::optional<Media::VideoInfo> getAudio()
		{
			return audio;
		}

		inline const bool hasAudio()
		{
			return (audio? true : false);
		}


		inline const std::string getURL(const Media::VideoInfo& vi)
		{
			return URL + "/" + vi.BaseURL;
		}

	private:
		Video() = default;

		std::optional<int> maxHeight{ std::nullopt }, maxWidth{ std::nullopt }, maxFPS{ std::nullopt };
		std::optional<Media::VideoInfo> audio{ std::nullopt };
		std::vector<Media::VideoInfo> videos;

		// TODO: Serialization for VideoInfo types
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& boost::serialization::base_object<Link>(*this);
			ar& maxHeight;
			ar& maxWidth;
			ar& maxFPS;
		}

		using Link::GetContent;
	};
};
