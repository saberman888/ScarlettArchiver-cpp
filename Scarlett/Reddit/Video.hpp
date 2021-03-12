#pragma once

#include "BaseTypes/Link.hpp"
#include "Comment.hpp"
#include <string_view>
#include <regex>
#include <tinyxml2.h>
#include <utility>
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
		void Mux(std::filesystem::path destination);

		/**
		* Checks if the provided json has an is_video tag, and if it is in fact: a boolean.
		* It checks for the values: is_value and post_hint.
		*/
		static bool IsVideo(const JSON::value& json);

		bool operator==(Video& other);
		bool operator!=(Video& other);

		inline const size_t getTotalVideos()
		{
			return videos.size();
		}

		inline const std::vector<std::tuple<int, std::string>> getVideos()
		{
			return videos;
		}

		inline const std::string getAudioURL()
		{
			return audio.value_or("null");
		}

		inline const bool hasAudio()
		{
			return audio.has_value();
		}
	private:
		Video() = default;
		void Fetch();

		std::optional<std::string> audio{ std::nullopt };
		std::vector<std::tuple<int, std::string>> videos;

		// TODO: Serialization for VideoInfo types
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& boost::serialization::base_object<Link>(*this);
		}

		void AddVideo(const std::tuple<int, std::string> video);

		using Link::GetContent;
	};
};
