#pragma once

#include "Postable.hpp"
#include <boost/serialization/base_object.hpp>

namespace Scarlett::Reddit::BaseTypes
{
	/*
	* TextPost serves as a base class for posts and comments with text in them.
	*/
	class TextPost : public Postable
	{
	public:
		TextPost() {}
		TextPost(const JSON::value& json);

		bool operator==(TextPost& other);
		bool operator!=(TextPost& other);

		String Text;	
		void Read(const JSON::value& json);

	private:
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			using namespace boost::serialization;
			ar& make_nvp("Postable", base_object<Postable>(*this));
			ar& make_nvp("Text", Text);
		}

	};

};
