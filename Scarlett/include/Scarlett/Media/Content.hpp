#pragma once

#include <boost/optional.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/serialization/optional.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/export.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <cpprest/http_client.h>
#include <cpprest/base_uri.h>
#include <cstddef>
#include <optional>
#include <string>
#include <regex>
#include <utility>
#include <filesystem>
#include <fstream>
#include "../Internal/Exceptions.hpp"
#include "../Internal/Helpers.hpp"

namespace Scarlett {



	namespace Media {
		class Content
		{
		public:
			Content() {}
			Content(const String& URL);
			Content(const URI& URL)
			{
				this->URL = URL;
			}

			Content(const URI& uri, const std::filesystem::path destination)
			{
				this->URL = uri;
				this->location = destination;
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

			inline const std::string GetURLString()
			{
				return toString(URL.to_string());
			}

			inline void SetURL(const URI uri)
			{
				URL = uri;
			}

			inline const URI GetURL()
			{
				return URL;
			}

			inline const std::filesystem::path GetPath()
			{
				return location;
			}

			inline void SetPath(const std::filesystem::path destination)
			{
				location = destination;
			}

			inline void Write(const std::string& filename)
			{
				auto full_path = location / filename;
				std::ofstream out(full_path.string(), std::ios::binary | std::ios::out);
				for (auto& data : GetContent())
					out << data;
			}


			StatusCode FetchContent(std::optional<URI> URL = std::nullopt);

			std::string Extension();

			inline std::string ContentType()
			{
				return _ContentType.empty() ? std::string() : _ContentType[0];
			}

			inline Size ContentSize()
			{
				return _ContentSize.get_value_or(0);
			}


		private:

			friend class boost::serialization::access;
			template<class Archive>
			void serialize(Archive& ar, const unsigned int version)
			{
				using namespace boost::serialization;
				ar& make_nvp("ContentInfo", _ContentType);
				ar& make_nvp("ContentSize", _ContentSize);
				auto url = toString(URL.to_string());
				ar& make_nvp("URL", url);
			}


			std::optional<web::http::http_response> Response{ std::nullopt };
			std::vector<std::string> _ContentType;
			boost::optional<Size> _ContentSize{ 0 };
			URI URL;
			std::filesystem::path location;
		};
	}
}

