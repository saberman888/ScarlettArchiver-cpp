#pragma once

#include "BaseTypes/TextPost.hpp"
#include <boost/serialization/export.hpp>
#include <boost/serialization/base_object.hpp>

namespace Scarlett::Reddit
{
	/*
	* SelfPost takes TextPost with Linkable traits, and that serves as a class for self posts
	*/
    class SCDLL SelfPost : public TextPost
	{
	public:
		SelfPost(const JSON::value& json);

		static inline bool IsSelfPost(const JSON::value& json)
		{
			return (json.has_string_field("is_self"_u) && json.at("is_self"_u).as_bool());
		}
	private:
		SelfPost() {};
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive& ar, const unsigned int version);
	};

};
