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

		const int Sum();

		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive& ar, const unsigned int version);
	};
}
