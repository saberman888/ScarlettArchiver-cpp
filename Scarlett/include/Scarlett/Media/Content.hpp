#pragma once

#include <boost/optional.hpp>
#include <boost/serialization/access.hpp>
#include <optional>
#include <string>
#include <utility>
#include <filesystem>
#include "../Internal/Types.hpp"
namespace Scarlett {



	namespace Media {
		class SCDLL Content
		{
		public:
			Content();
			Content(const String& URL);
			Content(const URI& URL);
			Content(const URI& uri, const std::filesystem::path destination);
			~Content();

			const auto GetContent();

			const std::string GetStringContent(bool ignore_content_type = true);

			const auto GetJSONContent();

			const String GetURLString();

			void SetURL(const URI& uri);

			const URI GetURL();

			const std::filesystem::path GetPath();

			void SetPath(const std::filesystem::path destination);

			void Write(const String& filename);


			const StatusCode FetchContent(std::optional<URI> URL = std::nullopt);

			const String Extension();

			const String ContentType();

			const Size ContentSize();


		private:

			friend class boost::serialization::access;
			template<class Archive>
			void serialize(Archive& ar, const unsigned int version);

			std::optional<HttpResponse> Response{ std::nullopt };
			std::vector<String> _ContentType;
			boost::optional<Size> _ContentSize{ 0 };
			URI URL;
			std::filesystem::path location;
		};
	}
}

