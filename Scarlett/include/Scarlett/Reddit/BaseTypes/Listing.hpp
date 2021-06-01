#pragma once

#include "../../Internal/Helpers.hpp"
#include "../../Client/OAuth2.hpp"
#include <optional>
#include <type_traits>

namespace Scarlett::Reddit::BaseTypes
{
  template<typename T>
  class Listing
  {
    public:
      Listing() = default;

      virtual void Next();
      virtual bool HasNext();
    protected:
      
      virtual void Read(const JSON::value& source);  
  };
};
