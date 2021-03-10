#pragma once

#include "BaseTypes/Link.hpp"
#include "Comment.hpp"

#include <string_view>
#include <regex>

namespace Scarlett::Reddit
{

	class Video : public BaseTypes::Link
	{
	public:
		Video(const JSON::value& json);

		void Fetch();
		void Mux(std::filesystem::path destination);

		/**
		* Checks if the provided json has an is_video tag, and if it is in fact: a boolean.
		*/
		static bool IsVideo(const JSON::value& json);

		bool operator==(Video& other);
		bool operator!=(Video& other);

		inline bool HasAudio() {
			return (Audio? true : false);
		}

		inline std::string GetVideoURL()
		{
			return VideoURL;
		}

		inline std::string GetAudioURL()
		{
			return URL + "/" + Audio.value();
		}

	private:
		Video() = default;
		void FetchDash();

		std::string DashURL;
		std::string VideoURL;
		std::optional<std::string> Audio{ std::nullopt };

		bool IsMP4(const std::string& MPEGManifestData);
		bool CheckforAudio(const std::string& MPEGManifestData);
		void ReadDash(const std::string& data);

		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& boost::serialization::base_object<Link>(*this);
			ar& VideoURL;
			ar& DashURL;
			ar& Audio.value_or("(null)");
		}
		using Link::GetContent;
	};
};
