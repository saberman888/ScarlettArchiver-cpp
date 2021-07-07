#pragma once

#include "Thing.hpp"
#include "../../Internal/Types.hpp"

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

		void Read(const JsonValue& json);
		const String getText();
	protected:
		TextPost();
	private:
		String Text;
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive& ar, const unsigned int version);
	};

};
