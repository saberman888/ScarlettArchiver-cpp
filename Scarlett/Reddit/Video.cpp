#include "Video.hpp"
#include <iostream>
BOOST_CLASS_EXPORT(Scarlett::Reddit::Video)


namespace Scarlett::Reddit
{
	Video::Video(const JSON::value& json) : BaseTypes::Link(json)
	{
	}

	void Video::Fetch()
	{
		auto dashData = Download(URL + "/DASHPlaylist.mpd");
		if (dashData.status_code() == 200)
		{
			// Extract the string from the response handle
			auto xmlData = dashData.extract_utf8string(true).get();

			tinyxml2::XMLDocument doc;
			doc.Parse(xmlData.c_str());

			auto Root = doc.RootElement()->FirstChildElement();

			for (tinyxml2::XMLElement* Adap = Root->FirstChildElement(); Adap != nullptr; Adap = Adap->NextSiblingElement())
			{
				std::string mimeType;
				if (Adap->Attribute("mimeType"))
				{
					mimeType = Adap->Attribute("mimeType");
				}

				for (tinyxml2::XMLElement* Rep = Adap->FirstChildElement(); Rep != nullptr; Rep = Rep->NextSiblingElement())
				{
					// Sometimes mimeType isn't going to be present in an AdaptationSet, but
					// the Representative could have it instead.
					if (Rep->Attribute("mimeType"))
					{
						mimeType = Rep->Attribute("mimeType");
					}
					
					if (mimeType == "audio/mp4")
					{
						audio->mimeType = "audio/mp4";
						audio->Read(Rep);
					}
					else {
						Media::VideoInfo vi;
						vi.mimeType = "video/mp4";
						vi.Read(Rep);
						videos.push_back(vi);
					}

				}
			}

		}
		else {
			scarlettThrow("Failed to retrieve dash playlist data! Code: " + dashData.status_code());
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
