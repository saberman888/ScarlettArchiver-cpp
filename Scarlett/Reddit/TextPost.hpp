#pragma once

#include <string>
#include "Linkable.hpp"
#include "Postable.hpp"
#include <boost/config.hpp>
#include <boost/serialization/export.hpp>
#include <boost/serialization/base_object.hpp>
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
		TextPost(const JSON::value& json);

		bool operator==(TextPost& other);
		bool operator!=(TextPost& other);

		std::string Text;	
		void Read(const JSON::value& json);

	private:
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& boost::serialization::base_object<Postable>(*this);
			ar& Text;
		}

		};

	/*
	* SelfPost takes TextPost with Linkable traits, and that serves as a class for self posts
	*/
	class SelfPost : public TextPost, public Linkable
	{
	public:
		SelfPost(const JSON::value& json);

		bool operator==(SelfPost& other);
		bool operator!=(SelfPost& other);


		static inline bool IsSelfPost(const JSON::value& json)
		{
			return (json.has_string_field("is_self"_u) && json.at("is_self"_u).as_bool());
		}

	private:
		SelfPost(){}
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& boost::serialization::base_object<Linkable>(*this);
			ar& boost::serialization::base_object<TextPost>(*this);
		}	

	};
};
