#pragma once

#include "exceptions.hpp"
#include "StringOps.hpp"
#include <boost/archive/text_oarchive.hpp>

namespace JSON = web::json;

namespace ScarlettArchiver::RedditAsset
{
	/*
	* Linkable is an class that represents common values that Videos, Images, Galleries and Self Posts have
	*/
	class Linkable
	{
	public:
		Linkable() {}
		std::string Domain;
		std::string Title;
		std::string URL;
		virtual bool operator==(const Linkable& other);
		virtual bool operator!=(const Linkable& other); 
		void Read(const JSON::value& json);

	private:
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& Domain;
			ar& Title;
			ar& URL;
		}
	};

};
