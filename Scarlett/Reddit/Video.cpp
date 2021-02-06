#include "Video.hpp"
#include <iostream>
BOOST_CLASS_EXPORT(ScarlettArchiver::RedditAsset::Video);


namespace ScarlettArchiver::RedditAsset
{
	Video::Video(const nlohmann::json& json) : DashPlaylistUrl(), AudioURL(),
		HasAudio(false)
	{
		RedditCommon::initLog();
		Read(json);
	}

	void Video::GetVideoInfo()
	{
		log->info("Attempting to get additional video information");
		log->info("Connecting to https://reddit.com/" + Id + ".json");
		State redditVideo = ScarlettArchiver::Download("https://reddit.com/" + Id + ".json");
		if (redditVideo.AllGood())
		{
			try {
				log->info("Reading...");
				nlohmann::json root = nlohmann::json::parse(redditVideo.buffer);
				auto post = root.at(0).at("data").at("children").at(0).at("data");

				auto redditVideo = post.at("secure_media").at("reddit_video");
				//URL = redditVideo.at("fallback_url").get<std::string>();
				DashPlaylistUrl = redditVideo.at("dash_url").get<std::string>();
				log->info("DASH URL: " + DashPlaylistUrl);
			}
			catch (nlohmann::json::exception& e) {
				log->error("Exception triggered: nlohmann::json");
				log->error(e.what());
				log->error("Rethrowing...");
				std::throw_with_nested(ScarlettArchiver::ScarlettPostException("Failed to extract JSON", this->Id));
			}

			log->info("Success!");
			State dashDownload = ScarlettArchiver::Download(DashPlaylistUrl);
			if (dashDownload.AllGood())
			{
				log->info("Reading...");
				if (std::regex_search(dashDownload.buffer, std::regex("<BaseURL>[DASH_]?audio[.mp4]?</BaseURL>"))) {
					HasAudio = true;
					log->info("Video has audio");
				}
				else {
					log->info("Video does not have audio");
				}
			}
		}
		else {
			log->error("Failed to download DASH");
			log->info("Throwing exception");
			throw ScarlettArchiver::ScarlettException("Failed to download DASH");
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
		if (!video.AllGood())
		{
			std::cerr << "Error, Failed to download Video" << std::endl;
			std::cerr << video.HttpState << " " << video.Message << std::endl;
			return false;
		} else{
			log->info("Downloaded video for " + Id);
		}

		if (HasAudio) {
			auto audio = ScarlettArchiver::Download(AudioURL);
			if (!audio.AllGood()) {
				std::cerr << "Error, Failed to download Audio" << std::endl;
				std::cerr << audio.HttpState << " " << audio.Message << std::endl;
				return false;
			}
			log->info("Downloaded audio for " + Id);
		}
		return true;
	}

	bool Video::IsVideo(const nlohmann::json& json)
	{
		return (json.contains("is_video") && json.at("is_video").is_boolean() && json.at("is_video").get<bool>());
	}

	void Video::Read(const nlohmann::json& json, bool ReadDomain)
	{
		try {
			RedditCommon::Read(json);
			// TODO: PushShift doesn't preserve media json structure that holds the information to get the dash file
			//auto redditVideo = json.at("media").at("reddit_video");
			//URL = redditVideo.at("fallback_url").get<std::string>();
			//DashPlaylistUrl = redditVideo.at("dash_url").get<std::string>();
		}
		catch (nlohmann::json::exception& e) {
			std::throw_with_nested(ScarlettArchiver::ScarlettPostException("Failed to extract JSON", this->Id));
		}
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
