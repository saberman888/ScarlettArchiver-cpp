#pragma once

#include <string>
#include "Linkable.hpp"
#include "Postable.hpp"
#include <boost/serialization/export.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

namespace ScarlettArchiver::RedditAsset
{

	class TextPost : public Postable, private Linkable
	{
	public:
		/**
		* An empty constructor because, boost's serialization requires it.
		*/
		TextPost(){}
		TextPost(const nlohmann::json& json);

		bool operator==(TextPost& other);
		bool operator!=(TextPost& other);

		const std::string Text;
		static inline bool IsTextPost(const nlohmann::json& json)
		{
			return (json.contains("is_self") && json.at("is_self").get<bool>());
		}
	protected:
		void Read(const nlohmann::json& json);
	private:
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& boost::serialization::base_object<Linkable>(*this);
			ar& boost::serialization::base_object<Postable>(*this);
			ar& Text;
		}
	};
};
