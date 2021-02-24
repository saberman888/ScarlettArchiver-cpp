#include "AccessData.hpp"

namespace Vun::Reddit
{
    template<Client C>
    std::string AccessData<C>::GetScopes()
	{
		std::string returnString;
		for (auto s : Scopes)
		{
			returnString += stringScopes[s];
			returnString += curl_escape(" ", sizeof " ");
		}
		return returnString;
	}
};
