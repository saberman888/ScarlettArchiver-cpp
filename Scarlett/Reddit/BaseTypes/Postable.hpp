#pragma once

#include <ctime>
#include <algorithm>
#include "../../Internal/exceptions.hpp"
#include "../../Internal/Helpers.hpp"
#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/serialization/nvp.hpp>

namespace JSON = web::json;

namespace Scarlett::Reddit::BaseTypes
{
	/*
	* Postable is an class that represents common values that Videos, Images, Galleries, Self Posts and Comments have
	*/
	class Postable
	{
	public:
		Postable() {};
		time_t CreatedUTC{ 0L };
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
			using namespace boost::serialization;
			ar& make_nvp("CreationDate", CreatedUTC);
			ar& make_nvp("Author", Author);
			ar& make_nvp("Id", Id);
			ar& make_nvp("Permalink", Permalink);
		}
	};

}
