#include "Scarlett/Reddit/SubredditMetadata.hpp"
#include "Scarlett/Internal/Helpers.hpp"
#include "Scarlett/Reddit/Video.hpp"
#include "Scarlett/Reddit/Galleries.hpp"
#include <boost/serialization/export.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <regex>
#include "Scarlett/Internal/Serializable.hpp"
#include <iostream>
BOOST_CLASS_EXPORT_GUID(Scarlett::Reddit::SubredditMetadata, "Metadata")

namespace Scarlett
{ 
	namespace Reddit
	{
        template<class Archive>
        void SubredditMetadata::serialize(Archive& ar, const unsigned int version)
        {
            using namespace boost::serialization;
            ar& make_nvp("TargetSubreddit", _Subreddit);
            ar& make_nvp("StartDate", StartDate);
            ar& make_nvp("EndDate", EndDate);
            ar& make_nvp("DatePointer", DatePointer);
        }
        template void SubredditMetadata::serialize<boost::archive::xml_oarchive>(boost::archive::xml_oarchive& ar, const unsigned int version);
        template void SubredditMetadata::serialize<boost::archive::xml_iarchive>(boost::archive::xml_iarchive& ar, const unsigned int version);

        SubredditMetadata::SubredditMetadata() = default;

        SubredditMetadata::SubredditMetadata(const String Subreddit, const String StartTime, const String EndTime)
        {
            InitializeDates(StartTime, EndTime);
            this->_Subreddit = Subreddit;
        }

        void SubredditMetadata::setSubreddit(const String& name)
        {
            _Subreddit = name;
        }

        const String SubredditMetadata::Subreddit()
        {
            return _Subreddit;
        }

        void SubredditMetadata::setStart(const Timestamp date)
        {
            StartDate = date;
        }

        const SubredditMetadata::Timestamp SubredditMetadata::Start()
        {
            return StartDate;
        }

        void SubredditMetadata::setEnd(const Timestamp date)
        {
            EndDate = date;
        }

        const SubredditMetadata::Timestamp SubredditMetadata::End()
        {
            return EndDate;
        }

        const SubredditMetadata::Timestamp SubredditMetadata::Position()
        {
            return DatePointer;
        }

        void SubredditMetadata::setPosition(const Timestamp date)
        {
            DatePointer = date;
        }

        void SubredditMetadata::InitializeDates(std::optional<String> Start, std::optional<String> End)
        {

            const auto match = [&Start, &End](const std::string pattern) -> bool { return (std::regex_match(toString(*Start), std::regex(pattern)) && std::regex_match(toString(*End), std::regex(pattern))); };

            // Check if the Start date and End date are present
            if (!Start)
            {
                // If Start is not present, assign it to when Reddit first emerged since we have no idea when the Subreddit emerged
                tm autoDate{ 0 };
                autoDate.tm_year = 2005;
                autoDate.tm_mday = 23;
                autoDate.tm_mon = 07;

                StartDate = mktime(&autoDate);
            }
            else {
                /*
                * Supported formats:
                *
                * 1. YYYY:MM:DD
                * 2. YYYY.MM.DD
                * 3. YYYY-MM-DD
                * 4. YYYMMDD
                * 5. 2020/01/01

                */
                std::string format;
                if (match("20[0-9][0-9](-|\\.|:|\\/)?[0-1]?[1-9](-|\\.|:|\\/)?[0-3]?[0-9]"))
                {
                    if (match("20[0-9][0-9][0-1][0-9][0-3][0-9]"))
                    {
                        format = "%Y%m%d";
                    }
                    else if (match("20[0-9][0-9]-[0-1][1-9]-[0-3][0-9]")) {
                        format = "%Y-%m-%d";
                    }
                    else if (match("20[0-9][0-9]:[0-1][1-9]:[0-3][0-9]")) {
                        format = "%Y:%m:%d";
                    }
                    else if (match("20[0-9][0-9].[0-1][1-9].[0-3][0-9]")) {
                        format = "%Y.%m.%d";
                    }
                    else if (match("20[0-9][0-9]/[0-1][1-9]/[0-3][0-9]")) {
                        format = "%Y/%m/%d";
                    }
                    else {
                        throw std::runtime_error("Not a valid date format");
                    }
                }
                else {
                    throw std::runtime_error("Not a valid date format");
                }

                if (Start) {
                    tm tempStart{ 0 };
                    Scarlett::strptime(toString(*Start).c_str(), format.c_str(), &tempStart);
                    StartDate = mktime(&tempStart);
                }
                if (End) {
                    tm tempEnd{ 0 };
                    Scarlett::strptime(toString(*End).c_str(), format.c_str(), &tempEnd);
                    EndDate = mktime(&tempEnd);
                }
            }
            this->DatePointer = this->StartDate;
        }
    }
	template class Scarlett::Serializable<Reddit::SubredditMetadata>;
}
