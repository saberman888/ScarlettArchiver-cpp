// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

// add headers that you want to pre-compile here
#include <string>
#include <exception>
#include <stdexcept>
#include <iosfwd>
#include <utility>
#include <iomanip>
#include <filesystem>
#include <fstream>
#include <type_traits>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <cstddef>
#include <optional>
#include <regex>
#include <utility>
#include <fstream>
#include <ctime>
#include <map>
#include <utility>
#include <cstring>
#include <chrono>
#include <thread>
#include <algorithm>

#include <boost/serialization/shared_ptr.hpp>
#include <boost/optional.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/serialization/optional.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/export.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/optional.hpp>
#include <boost/serialization/nvp.hpp>

#include <cpprest/http_client.h>
#include <cpprest/json.h>
#include <cpprest/base_uri.h>
#include <cpprest/http_msg.h>
#include <cpprest/http_headers.h>
#include <cpprest/oauth2.h>

#include <tinyxml2.h>

#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_sinks.h"

#include "Scarlett_def.hpp"
#include "ScarlettConstants.h"
#include "Types.hpp"
#include "Logger.hpp"
#include "Exceptions.hpp"
#include "Serializable.hpp"
#include "../Media/Content.hpp"
#include "../Media/Imgur.hpp"
#include "../Reddit/BaseTypes/Thing.hpp"
#include "../Reddit/BaseTypes/Link.hpp"
#include "../Reddit/BaseTypes/Listing.hpp"
#include "../Reddit/BaseTypes/TextPost.hpp"
#include "../Reddit/RedditStatistics.hpp"
#include "../Reddit/Comment.hpp"

#endif //PCH_H
