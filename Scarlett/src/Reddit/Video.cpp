#include "Scarlett/Reddit/Video.hpp"
#include "Scarlett/Internal/Exceptions.hpp"
#include <iostream>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/export.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <tinyxml2.h>
#include <utility>
BOOST_SERIALIZATION_SHARED_PTR(ScarletT::Reddit::Video);
BOOST_CLASS_EXPORT_GUID(Scarlett::Reddit::Video, "Video");


namespace Scarlett
{
    namespace Reddit
    {
        template<class Archive>
        void VideoInfo::serialize(Archive& ar, const unsigned int version)
        {
            using namespace boost::serialization;
            ar& make_nvp("Height", Height);
            ar& make_nvp("BaseURL", BaseURL);
        }
        template void VideoInfo::serialize<boost::archive::xml_oarchive>(boost::archive::xml_oarchive& ar, const unsigned int version);
        template void VideoInfo::serialize<boost::archive::xml_iarchive>(boost::archive::xml_iarchive& ar, const unsigned int version);

        template<class Archive>
        void Video::serialize(Archive& ar, const unsigned int version)
        {
            using namespace boost::serialization;
            ar& make_nvp("Link", base_object<Link>(*this));
            ar& make_nvp("Audio", audio);
            ar& make_nvp("VideoInformation", videos);
        }
        template void Video::serialize<boost::archive::xml_oarchive>(boost::archive::xml_oarchive& ar, const unsigned int version);
        template void Video::serialize<boost::archive::xml_iarchive>(boost::archive::xml_iarchive& ar, const unsigned int version);

        Video::Video(const JsonValue& json) : Link(json)
        {
            Fetch();
        }

        const size_t Video::TotalVideos() { return videos.size(); }
        const std::vector<VideoInfo> Video::GetVideos() { return videos; }
        const bool Video::HasAudio() { return audio.has_value(); }
        void Video::Fetch()
        {
            audio.emplace();
            auto dashData = Download(URL.GetURLString() + "/DASHPlaylist.mpd"_u);
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
                            audio.emplace(
                                toScarlettString(Rep->FirstChildElement("BaseURL")->GetText())
                                );
                        }
                        else {

                            auto video = std::make_tuple<int, std::string>(
                                std::stoi(Rep->Attribute("height")),
                                Rep->FirstChildElement("BaseURL")->GetText()
                                );
                            struct VideoInfo vi {
                                std::stoi(Rep->Attribute("height")),
                                toScarlettString(Rep->FirstChildElement("BaseURL")->GetText())
                            };
                            AddVideo(vi);
                        }
                    }
                }

            }
            else {
                scarlettThrow("Failed to retrieve dash playlist data! Code: " + std::to_string(dashData.status_code()));
            }
        }

        bool Video::IsVideo(const JsonValue& json)
        {
            return (json.has_boolean_field("is_video"_u) && json.at("is_video"_u).as_bool() && json.at("post_hint"_u).as_string() == "hosted:video"_u);
        }

        const String Video::GetAudioURL()
        {
            if (audio)
            {
                return URL.GetURLString() + "/"_u + audio.value();
            }
            else {
                scarlettThrow("This Video does not have audio!");
            }
            return String();
        }

        bool Video::operator==(Video& other)
        {
            return (Link::operator==(other) && audio.value_or("null"_u) == other.audio.value_or("null"_u));
        }

        bool Video::operator!=(Video& other)
        {
            return (Link::operator!=(other) && audio.value_or("null"_u) != other.audio.value_or("null"_u));
        }

        void Video::AddVideo(const VideoInfo video)
        {
            if (videos.size() != 0)
            {
                for (std::vector<VideoInfo>::iterator it = videos.begin(); it != videos.end(); it++)
                {
                    if ((*it).Height > video.Height) {
                        videos.emplace(it, video);
                        return;
                    }
                }
            }
            videos.emplace_back(video);
        }

        void Video::Mux(std::filesystem::path source)
        {
            using namespace Scarlett;
            String filename = Id + ".mkv"_u;
            String ffmpegCommand = "ffmpeg -y -i {video_source} {audio_source} -c copy {destination_video}"_u;

            std::filesystem::path video = source / (Id + "_video.mp4"_u);
            std::filesystem::path audio = source / (Id + "_audio.mp4"_u);

            ffmpegCommand = SearchAndReplace(ffmpegCommand, "{video_source}"_u, toScarlettString(video.string()));
            ffmpegCommand = SearchAndReplace(ffmpegCommand, "{audio_source}"_u, toScarlettString(audio.string()));
            ffmpegCommand = SearchAndReplace(ffmpegCommand, "{destination}"_u, toScarlettString(source.string()));


            std::system(toString(ffmpegCommand).c_str());
            std::filesystem::remove(video);
            std::filesystem::remove(audio);

        }
    }
	template class Scarlett::Serializable<Reddit::Video>;
    template class Scarlett::Serializable<struct Reddit::VideoInfo>;
};
