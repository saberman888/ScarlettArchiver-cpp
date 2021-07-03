#pragma once
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/serialization/nvp.hpp>
#include "Video.hpp"
#include "SelfPost.hpp"
#include "Galleries.hpp"

namespace Scarlett::Reddit
{
	struct RedditStatistics
	{
		int Videos{ 0 }, Links{ 0 }, SelfPosts{ 0 }, Galleries{ 0 };

		template<class T>
		void Append();

		inline int Sum()
		{
			return Videos + Links + SelfPosts + Galleries;
		}


		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive& ar, const unsigned int version);
	};

	template<class T>
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
	};
}
