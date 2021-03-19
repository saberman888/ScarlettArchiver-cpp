#pragma once

#include "BaseTypes/TextPost.hpp"
#include "BaseTypes/Linkable.hpp"
#include <boost/serialization/export.hpp>
#include <boost/serialization/base_object.hpp>

namespace Scarlett::Reddit
{
	/*
	* SelfPost takes TextPost with Linkable traits, and that serves as a class for self posts
	*/
	class SelfPost : public BaseTypes::TextPost, public BaseTypes::Linkable
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
		SelfPost() = default;
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			using namespace boost::serialization;
			ar& make_nvp("Linkable", base_object<BaseTypes::Linkable>(*this));
			ar& make_nvp("Linkable", base_object<BaseTypes::TextPost>(*this));
		}

	};

};
