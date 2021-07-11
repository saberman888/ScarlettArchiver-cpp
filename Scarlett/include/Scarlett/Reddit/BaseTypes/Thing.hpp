#pragma once


#include <boost/serialization/access.hpp>
#include <ctime>
#include "../../Internal/Serializable.hpp"
#include "../../Internal/Types.hpp"

namespace Scarlett::Reddit
{
    class SCDLL Thing
	{
	public:
		using Timestamp = time_t;

		bool operator==(const Thing& thing);
		bool operator!=(const Thing& thing);

		const String getName();
		const String getId();
		const String getAuthor();
		const String getPermalink();
		const Timestamp getCreatedUTCTime();
		const Timestamp getCreatedLocalTime();

	protected:
		Thing();
		Thing(const JsonValue& value);

		String Name;
		String Id;
		String Author;
		String Permalink;
		Timestamp CreatedUTC{ 0L };
		Timestamp CreatedLocal{ 0L };

	private:
		virtual void Read(const JsonValue& value);

		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive& ar, const unsigned int version);
	};

    bool IsSelf(const JsonValue val);
}
