#pragma once

#include "BaseTypes/Link.hpp"
#include "Scarlett/Misc.hpp"
#include "Comment.hpp"

#include <regex>

namespace Scarlett::RedditAsset
{
	class Video : public BaseTypes::Link
	{
	public:
		Video() : _HasAudio(false){}
		void GetVideoInfo();
		void Mux(std::filesystem::path destination);

		/**
		* Checks if the provided json has an is_video tag, and if it is in fact: a boolean.
		*/
		static bool IsVideo(const JSON::value& json);

		bool operator==(Video& other);
		bool operator!=(Video& other);

		inline bool HasAudio() {
			return _HasAudio;
		}

		inline std::string GetVideoURL()
		{
			return URL + "/" + VideoURL + (_IsMP4? ".mp4" : std::string());
		}

		inline std::string GetAudioURL()
		{
			return URL + "/" + AudioURL + (_IsMP4? ".mp4" : std::string());
		}

	private:
		std::string AudioURL, VideoURL, MPEGManifestURL;
		bool _HasAudio, _IsMP4;

		bool IsMP4(const std::string& MPEGManifestData);
		bool CheckforAudio(const std::string& MPEGManifestData);

		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& boost::serialization::base_object<Link>(*this);
			ar& AudioURL;
			ar& VideoURL;
			ar& MPEGManifestURL;
			ar& _HasAudio;
		}
		using Link::GetContent;
	};
};
