#pragma once

#include <string>
#include <ctime>
#include <algorithm>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <cpprest/json.h>
#include "cpprest/details/basic_types.h"
#include "exceptions.hpp"

namespace JSON = web::json;
typedef std::wstring String;

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
		String Author;
		String Id;
		String Permalink;
		virtual bool operator==(Postable& other);
		virtual bool operator!=(Postable& other);
		inline struct tm GetUTCTime()
		{
			return *std::gmtime(&CreatedUTC);
		}

		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& CreatedUTC;
			ar& Author;
			ar& Id;
			ar& Permalink;
		}


		void Read(const JSON::value& json);
		bool timediff(time_t end, time_t begin);
	};

}
