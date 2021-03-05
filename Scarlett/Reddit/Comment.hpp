#pragma once

#include <string>
#include <memory>
#include <vector>
#include <optional>
#include "Misc.hpp"
#include "TextPost.hpp"
#include "exceptions.hpp"
#include <boost/config.hpp>
#include <boost/serialization/export.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

namespace ScarlettArchiver::RedditAsset
{
	
	class Comment : public TextPost
	{
	public:
		Comment(const std::string& ParentId);
		Comment(const JSON::value& json, std::optional<std::string> ParentId = std::nullopt);
		std::vector<std::unique_ptr<Comment>> replies; 
		void GetRedditComments();
		void GetPushShiftComments();

	private:
		Comment(){}
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& boost::serialization::base_object<TextPost>(*this);
			auto ParentIdstr = ParentId.value_or("(null)");
			ar& ParentIdstr;
		}


		std::optional<std::string> ParentId;
		void Read(const JSON::value& json);
	};	
	using CommentTree = std::vector<std::unique_ptr<Comment>>;
};
