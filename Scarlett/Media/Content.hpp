#pragma once

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
			std::optional<web::http::http_response> Response{ std::nullopt };
			std::vector<std::string> _ContentType;
			Size _ContentSize{ 0 };
			ScarlettURL URL;
		};
	}
}

