#pragma once

#include "Thing.hpp"
#include <boost/serialization/base_object.hpp>

namespace Scarlett::Reddit
{
	/*
	* TextPost serves as a base class for posts and comments with text in them.
	*/
    class SCDLL TextPost : public Thing
	{
	public:
		
		TextPost(const JsonValue& json);

		bool operator==(TextPost& other);
		bool operator!=(TextPost& other);

		void Read(const JSON::value& json);
		inline const String getText() { return Text; }
	protected:
		TextPost() {}
	private:
		String Text;
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			using namespace boost::serialization;
			ar& make_nvp("Thing", base_object<Thing>(*this));
			ar& make_nvp("Text", Text);
		}

	};

};
