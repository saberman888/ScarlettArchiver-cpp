#pragma once

#include "BaseTypes/TextPost.hpp"
#include "../Internal/Serializable.hpp"
namespace Scarlett::Reddit
{
	/*
	* SelfPost takes TextPost with Linkable traits, and that serves as a class for self posts
	*/
    class SCDLL SelfPost : public TextPost
	{
	public:
		SelfPost(const JsonValue& json);

		static bool IsSelfPost(const JsonValue& json);
	private:
        friend class Scarlett::Serializable<SelfPost>;
		SelfPost();
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive& ar, const unsigned int version);
	};

};
