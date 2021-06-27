#include "Scarlett/Client/Reddit.hpp"

namespace Scarlett::Client
{

  #define _SCARLETT_REDDIT_SCOPE
  #define DAT(a_, b_) const reddit_scope reddit_scopes::a_(SCARLETT_WIDEN(b_));
  #include "Scarlett/Internal/ScarlettConstants.h"
  #undef _SCARLETT_REDDIT_SCOPE
  #undef DAT
}
