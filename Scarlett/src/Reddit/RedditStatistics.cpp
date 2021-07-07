#include "Scarlett/Reddit/RedditStatistics.hpp"
BOOST_CLASS_EXPORT_GUID(Scarlett::Reddit::RedditStatistics, "RedditStatistics")
namespace Scarlett::Reddit
{
	template<class Archive>
	void RedditStatistics::serialize(Archive& ar, const unsigned int version)
	{
		using namespace boost::serialization;
		ar& make_nvp("Videos", Videos);
		ar& make_nvp("Links", Links);
		ar& make_nvp("SelfPosts", SelfPosts);
		ar& make_nvp("Galleries", Galleries);
	}

	template void RedditStatistics::serialize<boost::archive::xml_oarchive>(boost::archive::xml_oarchive& ar, const unsigned int version);
	template void RedditStatistics::serialize<boost::archive:xml_iarchive>(boost::archive:xml_iarchive& ar, const unsigned int version);

	template<typename T>
	void RedditStatistics::Append()
	{
		if constexpr (std::is_same<Gallery, T>::value)
		{
			Galleries += 1;
		}
		else if constexpr (std::is_same<SelfPost, T>::value) {
			SelfPosts += 1;
		}
		else if constexpr (std::is_same<Video, T>::value) {
			Videos += 1;
		}
		else {
			Links += 1;
		}
	}

	const int RedditStatistics::Sum() {
		return Videos + Links + SelfPosts + Galleries;
	}

	template class RedditStatistics::Append<Gallery>();
	template class RedditStatistics::Append<SelfPost>();
	template class RedditStatistics::Append<Video>();
	template class RedditStatistics::Append<Link>();
};
