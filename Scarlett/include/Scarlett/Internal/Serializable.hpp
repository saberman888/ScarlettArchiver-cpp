#pragma once
#include <filesystem>
#include "Scarlett/Internal/Types.hpp"

namespace Scarlett
{
    template<typename T>
    class SCDLL Serializable
    {
    public:
        static void Serialize(const std::filesystem::path destination, T& sourceObj, const std::string Tag);
        static T DeSerialize(const std::filesystem::path source, const std::string Tag);
    };

};
