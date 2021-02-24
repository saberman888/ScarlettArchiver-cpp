#pragma once

#include <string>
#include "Linkable.hpp"
#include "Postable.hpp"
#include <boost/serialization/export.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

namespace ScarlettArchiver::RedditAsset
{
	/*
	* TextPost serves as a base class for posts and comments with text in them.
	*/
	class TextPost : public Postable
	{
	public:
		/**
		* An empty constructor because, boost's serialization requires it.
		*/
		TextPost() {}
		TextPost(const nlohmann::json& json);

		bool operator==(TextPost& other);
		bool operator!=(TextPost& other);

		std::string Text;

		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& boost::serialization::base_object<Postable>(*this);
			ar& Text;
		}

		void Read(const nlohmann::json& json);
	};

	/*
	* SelfPost takes TextPost with Linkable traits, and that serves as a class for self posts
	*/
	class SelfPost : public TextPost, public Linkable
	{
	public:
		/**
		* An empty constructor because, boost's serialization requires it.
		*/
		SelfPost(){}
		SelfPost(const nlohmann::json& json);

		bool operator==(SelfPost& other);
		bool operator!=(SelfPost& other);


		static inline bool IsSelfPost(const nlohmann::json& json)
		{
			return (json.contains("is_self") && json.at("is_self").get<bool>());
		}


		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& boost::serialization::base_object<Linkable>(*this);
			ar& boost::serialization::base_object<TextPost>(*this);
		}	

	};
};
