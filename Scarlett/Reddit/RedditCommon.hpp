#pragma once

#include <string>
#include <ctime>
#include <algorithm>
#include "nlohmann/json.hpp"
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
/*#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/serialization/nvp.hpp>*/
#include "exceptions.hpp"
#include "Logger.hpp"


namespace ScarlettArchiver::RedditAsset
{
	/*
	* Postable is an class that represents common values that Videos, Images, Galleries, Self Posts and Comments have
	*/
	class Postable
	{
	public:
		Postable(){}
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

	protected:
		void Read(const nlohmann::json& json);
		bool timediff(unsigned int end, unsigned int begin);
	};

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
	private:
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& Domain;
			ar& Title;
			ar& URL;
		}

	protected:
		void Read(const nlohmann::json& json);
	};
};