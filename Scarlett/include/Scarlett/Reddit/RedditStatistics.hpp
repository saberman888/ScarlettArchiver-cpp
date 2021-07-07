#pragma once
#include <boost/serialization/access.hpp>
#include "../Internal/Serializable.hpp"


namespace Scarlett::Reddit
{
	struct RedditStatistics
	{
		int Videos{ 0 }, Links{ 0 }, SelfPosts{ 0 }, Galleries{ 0 };

		template<class T>
		void Append();

		const int Sum();

		template<typename T>
		friend void Serialize(const std::filesystem::path destination, T& sourceObj, const std::string Tag);

		template<typename T>
		T DeSerialize(const std::filesystem::path source, const std::string Tag);

		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive& ar, const unsigned int version);
	};
}
