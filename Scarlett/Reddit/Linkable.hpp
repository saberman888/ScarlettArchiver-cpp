#pragma once

#include <string>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <cpprest/json.h>
#include "exceptions.hpp"

namespace JSON = web::json;
typedef std::wstring String;

namespace ScarlettArchiver::RedditAsset
{
	/*
	* Linkable is an class that represents common values that Videos, Images, Galleries and Self Posts have
	*/
	class Linkable
	{
	public:
		/*
		* Empty constructor for boost serialization
		*/
		Linkable() {}
		String Domain;
		String Title;
		String URL;
		virtual bool operator==(const Linkable& other);
		virtual bool operator!=(const Linkable& other);

		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& Domain;
			ar& Title;
			ar& URL;
		}

		void Read(const JSON::value& json);
	};

};
