#include "Scarlett/Client/Reddit.hpp"

namespace Scarlett::Client
{

  #define _REDDITSCOPE
  #define DAT(a_, b_) const reddit_scope reddit_scopes::a_(b_);
  #include "Scarlett/Internal/ScarlettConstants.h"
  #undef _REDDITSCOPE
  #undef DAT
}
