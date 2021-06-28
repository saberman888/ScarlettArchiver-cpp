#include "Scarlett/Reddit/SelfPost.hpp"
BOOST_CLASS_EXPORT(Scarlett::Reddit::SelfPost);
BOOST_SERIALIZATION_SHARED_PTR(Scarlett::Reddit::SelfPost);

namespace Scarlett::Reddit
{
	SelfPost::SelfPost(const JSON::value& json) : TextPost(json)
	{
		TextPost::Read(json);
	}

};