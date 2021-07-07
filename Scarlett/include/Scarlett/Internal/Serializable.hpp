#pragma once
#include <filesystem>
#include "Scarlett/Internal/Types.hpp"

namespace Scarlett::Internal
{
	template<typename T>
	SCDLL void Serialize(const std::filesystem::path destination, T& sourceObj, const std::string Tag);

	template<typename T>
	SCDLL T DeSerialize(const std::filesystem::path source, const std::string Tag);

};
