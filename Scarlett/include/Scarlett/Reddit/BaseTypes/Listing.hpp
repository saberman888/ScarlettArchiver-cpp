#pragma once

#include "../../Internal/Helpers.hpp"
#include "../../Client/OAuth2.hpp"
#include <optional>
#include <type_traits>

namespace Scarlett::Reddit::BaseTypes
{
  template<typename T typename U = void>
  class Listing
  {
    public:
      virtual void Next();
      virtual bool HasNext();
    protected:
      Listing(const std::shared_ptr< OAuth2Helper<U> > m_oauth2_handle) : m_oauth2_helper(m_oauth2_handle){}
      virtual void Read(const JSON::value& source);
      std::enable_if_t< std::conditional_t<std::is_void_v<U>,  std::shared_ptr<OAuth2Helper<U>>> m_oauth2_handle{nullptr};
    private:
      Listing() = default;
  };
};
