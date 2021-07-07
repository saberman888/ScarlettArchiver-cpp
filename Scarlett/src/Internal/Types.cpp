#include "Scarlett/Internal/Types.hpp"
#include <vector>
#include <sstream>
#include <string>
#include <iosfwd>

#include <iomanip>
#include <fstream>

namespace Scarlett
{

	template<class CharT,
		class Traits = std::char_traits<CharT>,
		class Allocator = std::allocator<CharT>>
		std::vector<std::basic_string<CharT, Traits, Allocator>> splitString(std::basic_string<CharT, Traits, Allocator> data, CharT delimeter)
	{
		decltype(data) temp;
		std::vector<decltype(data)> returnList;
		std::basic_stringstream<CharT, Traits, Allocator> ss(data);

		while (std::getline(ss, temp, delimeter))
		{
			returnList.push_back(temp);
		}
		return returnList;
	}

	template<class CharT,
		class Traits = std::char_traits<CharT>,
		class Allocator = std::allocator<CharT>>
		std::basic_string<CharT, Traits, Allocator> SearchAndReplace(std::basic_string<CharT, Traits, Allocator> Input, const std::basic_string<CharT, Traits, Allocator> ToBeReplaced, const std::basic_string<CharT, Traits, Allocator> Replacement)
	{
		if (Input.find(ToBeReplaced) != std::basic_string<CharT, Traits, Allocator>::npos)
		{
			Input.replace(Input.find(ToBeReplaced), ToBeReplaced.size(), Replacement);
		}
		return Input;
	}

#ifdef __SCARLETT_USE_WIDE_STRINGS
	template std::basic_string<char> SearchAndReplace(std::basic_string<char> Input, const std::basic_string<char> ToBeReplaced, const std::basic_string<char> Replacement);
	template std::vector<std::basic_string<char>> splitString(std::basic_string<char> data, char delimeter);

#endif

	template std::vector<std::basic_string<String::value_type>> splitString(std::basic_string<String::value_type> data, String::value_type delimeter);
	template std::basic_string<String::value_type> SearchAndReplace(std::basic_string<String::value_type> Input, const std::basic_string<String::value_type> ToBeReplaced, const std::basic_string<String::value_type> Replacement);



	String toScarlettString(const std::string source)
	{
#ifdef __SCARLETT_USE_WIDE_STRINGS
		return conv::to_string_t(source);
#else
		return source;
#endif
	}

	std::string toString(const String& source) {
#ifdef __SCARLETT_USE_WIDE_STRINGS
		return utility::conversions::to_utf8string(source);
#else
		return source;
#endif
	}

	String operator""_u(const char* source, size_t csize)
	{
		return utility::conversions::to_string_t(std::string(source, csize));
	}


};