#pragma once

#include "TextPost.hpp"
#include "Linkable.hpp"
#include <boost/serialization/export.hpp>
#include <boost/serialization/base_object.hpp>

namespace ScarlettArchiver::RedditAsset
{
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
		SelfPost() {}
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& boost::serialization::base_object<Linkable>(*this);
			ar& boost::serialization::base_object<TextPost>(*this);
		}

	};

};