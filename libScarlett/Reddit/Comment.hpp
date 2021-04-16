#pragma once

#include <memory>
#include <optional>
#include "../Core/Misc.hpp"
#include "BaseTypes/TextPost.hpp"
#include <boost/serialization/export.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/serialization/optional.hpp>
namespace Scarlett::Reddit
{
	
	class Comment : public BaseTypes::TextPost
	{
	public:
		Comment(const std::string& ParentId);
		Comment(const JSON::value& json, boost::optional<std::string> ParentId = boost::none);
		std::vector<std::unique_ptr<Comment>> replies; 
		void GetRedditComments();

	private:
		Comment(){}
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			using namespace boost::serialization;
			ar& make_nvp("TextPost", base_object<BaseTypes::TextPost>(*this));
			ar& make_nvp("ParentId", ParentId);
		}

		boost::optional<std::string> ParentId{ boost::none };
		void Read(const JSON::value& json);
	};	
	using CommentTree = std::vector<std::unique_ptr<Comment>>;
};
