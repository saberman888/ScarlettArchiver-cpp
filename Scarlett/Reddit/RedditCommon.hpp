#pragma once

#include <string>
#include <ctime>
#include <filesystem>
#include <optional>
#include "nlohmann/json.hpp"
#include <fstream>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <omp.h>
/*#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/serialization/nvp.hpp>*/
#include "exceptions.hpp"
#include "Logger.hpp"
#include "Comment.hpp"



namespace ScarlettArchiver::RedditAsset
{

	enum class SerializationMethod
	{
		Text,
		Binary,
		XML
	};

	/**
		RedditCommon is an ancestor class for all postable Reddit components to inherit from. 
		It has common values that are shared between Links, Self Posts, Images, Videos, Galleries and Comments
	*/
	class RedditCommon
	{
	public:
		/**
		* A constructor with an empty signature. It only initializes SerializeAs and increments NumberOfInstances
		*/
		RedditCommon() : URL(std::nullopt), Title(std::nullopt), Domain(std::nullopt), CreatedUTC(0), SerializeAs(SerializationMethod::Text) {}
		void SerializeTo(SerializationMethod sm = SerializationMethod::Text);

		/**
			Common values that are shared between: Links, Self Posts, Images, Videos, Galleries and Comments
			URL, Title and Domain are optional values because, these tags are absent from Comments
		*/
		std::string Id, Author, Permalink;
		std::optional<std::string> URL, Title, Domain;

		// Time the post was created in Unix Epoch Time
		time_t CreatedUTC;

		virtual bool operator==(const RedditCommon& other);
		virtual bool operator!=(const RedditCommon& other);

		/**
		* Serializes the class using Boost's serialization library and writes it to a file
		* @param A directory to put the serializes class and a filename
		*/
		//void Write(std::filesystem::path destination, std::string filename);
		std::shared_ptr<spdlog::logger> log;
		void initLog();
	protected:
		/**
		* Parses and assigns values from Reddit's json into the class' values
		*/
		virtual void Read(const nlohmann::json& json);
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			if (SerializeAs == SerializationMethod::Text)
			{
				TextSerialize(ar, version);
			}
			else if (SerializeAs == SerializationMethod::XML) {
				//XmlSerialize(ar, version);
			}
		}

		SerializationMethod SerializeAs;
		template<class Archive>
		void TextSerialize(Archive& ar, const unsigned int version)
		{
			ar& URL.value();
			ar& Id;
			ar& Author;
			ar& Permalink;
			ar& Title.value();
			ar& Domain.value();
			ar& CreatedUTC;
		}

		/*template<class Archive>
		void XmlSerialize(Archive& ar, const unsigned int version)
		{
			ar& boost::serialization::make_nvp("URL", URL);
			ar& boost::serialization::make_nvp("Id", Id);
			ar& boost::serialization::make_nvp("Author", Author);
			ar& boost::serialization::make_nvp("Permalink", Permalink);
			ar& boost::serialization::make_nvp("Title", Title);
			ar& boost::serialization::make_nvp("Domain", Domain);
			ar& boost::serialization::make_nvp("CreatedUTC", CreatedUTC);
		}*/

	};
};
