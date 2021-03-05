#include "Video.hpp"
#include <iostream>
BOOST_CLASS_EXPORT(ScarlettArchiver::RedditAsset::Video);


namespace ScarlettArchiver::RedditAsset
{
	Video::Video(const JSON::value& json) : HasAudio(false)
	{

		Read(json);
	}

	Video::Video()
	{
		GetVideoInfo();
	}

	void Video::GetVideoInfo()
	{
		log->info("Attempting to get additional video information");
		log->info("Connecting to https://reddit.com/" + Id + ".json");
		auto redditVideo = ScarlettArchiver::Download("https://reddit.com/" + Id + ".json");
		if (redditVideo.status_code() == 200)
		{
			try {
				log->info("Reading...");
				auto root = redditVideo.extract_json().get();
				auto post = root.at(0).at(L"data").at(L"children").at(0).at(L"data");

				Link::Read(post);

				auto redditVideo = post.at(L"secure_media").at(L"reddit_video");
				DashPlaylistUrl = ToU8String(redditVideo.at(L"dash_url").as_string());
				log->info("DASH URL: " + DashPlaylistUrl);
			}
			catch (JSON::json_exception& e) {
				scarlettNestedThrow("Failed to extract JSON from Video, " + std::string(e.what()));
			}

			log->info("Success!");
			auto dashDownload = ScarlettArchiver::Download(DashPlaylistUrl);
			if (dashDownload.status_code() == 200)
			{
				log->info("Reading...");
				if (std::regex_search(ToU8String(dashDownload.extract_string().get()), std::regex("<BaseURL>[DASH_]?audio[.mp4]?</BaseURL>"))) {
					HasAudio = true;
					log->info("Video has audio");
				}
				else {
					log->info("Video does not have audio");
				}
			}
		}
		else {
			scarlettThrow("Failed to download DASH");
		}
	}

	bool Video::Download(std::filesystem::path destination)
	{
		size_t ending = URL.rfind("/");
		std::string audio_url = URL.substr(0, ending + 1);
		if (ScarlettArchiver::contains(URL, ".mp4"))
		{
			AudioURL = audio_url + "DASH_audio.mp4";
		}
		else {
			AudioURL = audio_url + "audio";
		}
		log->info("Video audio URL: " + AudioURL);

		auto video = ScarlettArchiver::Download(URL);
		if (!video.status_code())
		{
			std::cerr << "Error, Failed to download Video" << std::endl;
			std::cerr << video.status_code() << std::endl;
			return false;
		} else{
			log->info("Downloaded video for " + Id);
		}

		if (HasAudio) {
			auto audio = ScarlettArchiver::Download(AudioURL);
			if (!audio.status_code() == 200) {
				std::cerr << "Error, Failed to download Audio" << std::endl;
				std::cerr << audio.status_code() << std::endl;
				return false;
			}
			log->info("Downloaded audio for " + Id);
		}
		return true;
	}

	bool Video::IsVideo(const JSON::value& json)
	{
		return (json.has_boolean_field(L"is_video") && json.at(L"is_video").as_bool());
	}

	bool Video::operator==(Video& other)
	{
		return (Link::operator==(other) && other.HasAudio == HasAudio && other.DashPlaylistUrl == DashPlaylistUrl);
	}

	bool Video::operator!=(Video& other)
	{
		return (Link::operator!=(other) && other.HasAudio == HasAudio && other.DashPlaylistUrl == DashPlaylistUrl);
	}

	void Video::Mux(std::filesystem::path source)
	{
		using namespace ScarlettArchiver;
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
}
