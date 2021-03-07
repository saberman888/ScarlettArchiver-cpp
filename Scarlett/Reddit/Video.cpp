#include "Video.hpp"
#include <iostream>
BOOST_CLASS_EXPORT(Scarlett::Reddit::Video)


namespace Scarlett::Reddit
{
	void Video::GetVideoInfo()
	{
		log->info("Attempting to get additional video information");
		log->info("Connecting to https://reddit.com/" + Id + ".json");
		auto redditVideo = Scarlett::Download("https://reddit.com/" + Id + ".json");
		if (redditVideo.status_code() == 200)
		{
			try {
				log->info("Reading...");
				auto root = redditVideo.extract_json().get();
				auto post = root.at(0).at("data"_u).at("children"_u).at(0).at("data"_u);

				Link::Read(post);

				auto redditVideo = post.at("secure_media"_u).at("reddit_video"_u);
				MPEGManifestURL = ToU8String(redditVideo.at("dash_url"_u).as_string());
				log->info("DASH URL: " + MPEGManifestURL);
			}
			catch (JSON::json_exception& e) {
				scarlettNestedThrow("Failed to extract JSON from Video, " + std::string(e.what()));
			}
		}
		else {
			scarlettThrow("Failed to get video information for " + Id  + ", error: " + std::to_string(redditVideo.status_code()));
		}
	}
	 
	bool Video::IsVideo(const JSON::value& json)
	{
		return (json.has_boolean_field("is_video"_u) && json.at("is_video"_u).as_bool());
	}

	bool Video::operator==(Video& other)
	{
		return (Link::operator==(other) && other._HasAudio == _HasAudio && other.MPEGManifestURL == MPEGManifestURL);
	}

	bool Video::operator!=(Video& other)
	{
		return (Link::operator!=(other) && other._HasAudio == _HasAudio && other.MPEGManifestURL == MPEGManifestURL);
	}

	bool Video::IsMP4(const std::string& MPEGManifestData)
	{
		log->info("Determining if " + Id + " is an mp4 file...");
		if (std::regex_search(MPEGManifestData, std::regex("\b\.mp4"))) {
			log->info(this->Id + " is an MP4 file.");
			return true;
		}
		log->info(this->Id + " is not an MP4 file.");
		return false;
	}

	bool Video::CheckforAudio(const std::string& MPEGManifestData)
	{
		log->info("Determining if " + Id + " has any audio...");
		if (std::regex_search(MPEGManifestData, std::regex("<BaseURL>[DASH_]?audio[.mp4]?</BaseURL>")))
		{
			log->info(Id + " has audio.");
			return true;
		}
		log->info(Id + " doesn't have any audio.");
		return false;
	}

	void Video::Mux(std::filesystem::path source)
	{
		using namespace Scarlett;
		std::string filename = Id + ".mkv";
		std::string ffmpegCommand = "ffmpeg -y -i {video_source} {audio_source} -c copy {destination_video}";

		std::filesystem::path video = source / (Id + "_video.mp4");
		std::filesystem::path audio = source / (Id + "_audio.mp4");

		ffmpegCommand = SearchAndReplace(ffmpegCommand, "{video_source}", video.string());
		ffmpegCommand = SearchAndReplace(ffmpegCommand, "{audio_source}", audio.string());
		ffmpegCommand = SearchAndReplace(ffmpegCommand, "{destination}", source.string());


		std::system(ffmpegCommand.c_str());
		std::filesystem::remove(video);
		std::filesystem::remove(audio);

	}
};
