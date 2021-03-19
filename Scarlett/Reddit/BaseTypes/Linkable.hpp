#pragma once

#include "Core/exceptions.hpp"
#include "Core/StringOps.hpp"
#include <boost/archive/xml_oarchive.hpp>
#include <boost/serialization/nvp.hpp>

namespace JSON = web::json;

namespace Scarlett::Reddit::BaseTypes
{
	/*
	* Linkable is an class that represents common values that Videos, Images, Galleries and Self Posts have
	*/
	class Linkable
	{
	public:
		Linkable() = default;
		std::string Domain;
		std::string Title;
		std::string URL;
		virtual bool operator==(const Linkable& other);
		virtual bool operator!=(const Linkable& other); 
		void Read(const JSON::value& json);

	private:
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			using namespace boost::serialization;
			ar& make_nvp("Domain", Domain);
			ar& make_nvp("Title", Title);
			ar& make_nvp("URL", URL);
		}
	};

};
