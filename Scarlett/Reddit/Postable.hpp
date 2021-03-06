#pragma once

#include <ctime>
#include <algorithm>
#include "exceptions.hpp"
#include "StringOps.hpp"
#include <boost/archive/text_oarchive.hpp>
namespace JSON = web::json;

namespace ScarlettArchiver::RedditAsset
{
	/*
	* Postable is an class that represents common values that Videos, Images, Galleries, Self Posts and Comments have
	*/
	class Postable
	{
	public:
		Postable() {}
		time_t CreatedUTC;
		std::string Author;
		std::string Id;
		std::string Permalink;
		virtual bool operator==(Postable& other);
		virtual bool operator!=(Postable& other);
		inline struct tm GetUTCTime()
		{
			return *std::gmtime(&CreatedUTC);
		}

		void Read(const JSON::value& json);
		bool timediff(time_t end, time_t begin);
	private:
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& CreatedUTC;
			ar& Author;
			ar& Id;
			ar& Permalink;
		}

	};

}
