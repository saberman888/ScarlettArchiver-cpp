#pragma once

#include <string>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <nlohmann/json.hpp>
#include "exceptions.hpp"

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
		std::string Domain;
		std::string Title;
		std::string URL;
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

		void Read(const nlohmann::json& json);
	};

};
