#pragma once

#include <memory>
#include <optional>
#include "../Internal/Helpers.hpp"
#include "BaseTypes/TextPost.hpp"
#include <boost/serialization/export.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/serialization/optional.hpp>
namespace Scarlett::Reddit
{
	
	class Comment : public TextPost
	{
	public:
		Comment(const String& ParentId);
		Comment(const JSON::value& json, boost::optional<String> ParentId = boost::none);
		std::vector<std::unique_ptr<Comment>> replies; 
		void GetRedditComments();
		inline const Kind getType() { return Kind::Comment; }

	private:
		Comment(){}
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			using namespace boost::serialization;
			ar& make_nvp("TextPost", base_object<TextPost>(*this));
			ar& make_nvp("ParentId", ParentId);
		}

		boost::optional<String> ParentId{ boost::none };
		void Read(const JSON::value& json);
	};	
	using CommentTree = std::vector<std::unique_ptr<Comment>>;
};
