#pragma once

#include <boost/optional.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/serialization/optional.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/export.hpp>
#include <cpprest/http_client.h>
#include <cpprest/base_uri.h>
#include <cstddef>
#include <optional>
#include <string>
#include <regex>
#include "../Core/exceptions.hpp"
#include "../Core/Misc.hpp"
#include "../Core/StringOps.hpp"

namespace Scarlett {
	using StatusCode = web::http::status_code;
	using ScarlettURL = web::uri;
	using Size = utility::size64_t;


	namespace Media {
		class Content
		{
		public:
			Content(){}
			Content(const std::string& URL);
			Content(const ScarlettURL URL)
			{
				this->URL = URL;
			}

			inline const auto GetContent()
			{
				return Response->extract_vector().get();
			}

			inline const std::string GetStringContent(bool ignore_content = true)
			{
				return Response->extract_utf8string(ignore_content).get();
			}

			inline const auto GetJSONContent()
			{
				return Response->extract_json().get();
			}

			inline const std::string GetURL()
			{
				return u8(URL.to_string());
			}

			StatusCode FetchContent(std::optional<std::string> URL = std::nullopt);

			std::string Extension();
			std::string ContentType();
			Size ContentSize();


		private:

			friend class boost::serialization::access;
			template<class Archive>
			void serialize(Archive& ar, const unsigned int version)
			{
				using namespace boost::serialization;
				ar& make_nvp("ContentInfo", _ContentType);
				ar& make_nvp("ContentSize", _ContentSize);
				auto url = u8(URL.to_string());
				ar& make_nvp("URL", url);
			}


			std::optional<web::http::http_response> Response{ std::nullopt };
			std::vector<std::string> _ContentType;
			boost::optional<Size> _ContentSize{ 0 };
			ScarlettURL URL;
		};
	}
}

