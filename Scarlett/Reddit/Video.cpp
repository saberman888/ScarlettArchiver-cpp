#include "Video.hpp"
#include <iostream>
BOOST_CLASS_EXPORT(Scarlett::Reddit::Video)


namespace Scarlett::Reddit
{
	Video::Video(const JSON::value& json) : BaseTypes::Link(json)
	{
	}

	void Video::FetchDash()
	{
		auto dash = Download(DashURL);
		if (dash.status_code() == 200)
		{
			ReadDash(u8(dash.extract_string().get()));
		}
		else {
			scarlettThrow("Failed to download DASH for " + Id)
		}
	}

	void Video::Fetch()
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

				JSON::value redditVideo;
				if (post.has_field("secure_media"_u))
				{
					redditVideo = post.at("secure_media"_u).at("reddit_video"_u);
				}
				else if(post.has_field("media"_u)){
					redditVideo = post.at("media"_u).at("reddit_video"_u);
				}

				DashURL = u8(redditVideo.at("dash_url"_u).as_string());
				VideoURL = u8(redditVideo.at("fallback_url"_u).as_string());

				log->info("DASH URL: " + DashURL);
				log->info("Video URL: " + VideoURL);

				FetchDash();
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
		return (json.has_boolean_field("is_video"_u) && json.at("is_video"_u).as_bool() && json.at("post_hint"_u).as_string() == "hosted:video"_u);
	}

	bool Video::operator==(Video& other)
	{
		return (Link::operator==(other) && other.HasAudio() == HasAudio() && other.DashURL == DashURL);
	}

	bool Video::operator!=(Video& other)
	{
		return (Link::operator!=(other) && other.HasAudio() == HasAudio() && other.DashURL == DashURL);
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

	void Video::ReadDash(const std::string& data)
	{

		if (CheckforAudio(data)) {
			if (IsMP4(data))
			{
				Audio = "DASH_audio.mp4";
			}
			else {
				Audio = "audio";
			}
		}
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
