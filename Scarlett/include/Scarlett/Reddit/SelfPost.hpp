#pragma once

#include "BaseTypes/TextPost.hpp"

namespace Scarlett::Reddit
{
	/*
	* SelfPost takes TextPost with Linkable traits, and that serves as a class for self posts
	*/
    class SCDLL SelfPost : public TextPost
	{
	public:
		SelfPost(const JSON::value& json);

		static bool IsSelfPost(const JsonValue& json);
	private:
		SelfPost();
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive& ar, const unsigned int version);
	};

};
