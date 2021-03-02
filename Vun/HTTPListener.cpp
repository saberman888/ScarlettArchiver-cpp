#include "HTTPListener.hpp"

namespace Vun::Helpers::HTTP
{
	Socket::Socket(int port = 80)
	{
		struct addrinfo *result, hints;

		std::memset(&hints, 0, sizeof hints);
		hints.ai_flags = AI_PASSIVE;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_family = AF_UNSPEC;
		hints.ai_protocol = 0;
		int yes = 1;

		if (int status = getaddrinfo(NULL, std::to_string(port).c_str(), &hints, &result) != 0)
		{
			// TODO: Add error handling for failing to getaddrinfo
		}

		for (struct addrinfo* ptr = result; ptr != nullptr; ptr = ptr->ai_next)
		{
			SOCKET = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
			if (SOCKET == -1)
			{
				// TODO: Add error handling for failed to get a file descriptor for socket
			}

			if (setsockopt(SOCKET, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int) == -1)
			{
				// TODO: Missing error handling #3
			}

			if (bind(SOCKET, ptr->ai_addr, ptr->ai_addrlen) == -1)
			{
				// TODO: Missing error handling #4
			}

			break;
		}
		freeaddrinfo(result);
	}
	Vun::Helpers::HTTP::Socket::~Socket()
	{
		close(SOCKET);
	}
};

addrinfo* Vun::Helpers::Sockets::GetAddressInfo(const struct addrinfo* hints, std::optional<std::string> domain, int port)
{
	const char* name = (domain == std::nullopt) ? 0 : domain->c_str();
	struct addrinfo *returninfo;
#ifdef _WIN32
	auto result = getaddrinfo(name, std::to_string(port).c_str(), hints, &returninfo);
	if (result != 0)
	{
		return nullptr;
	}
#elif
	auto result = getaddrinfo(name, std::to_string(port).c_str(), &hints, &returninfo);
	if (result != 0)
	{
		return nullptr;
	}
#endif
	return returninfo;
}

SOCKETDESC Vun::Helpers::Sockets::InitSocket(struct addrinfo* info)
{
	return socket(info->ai_family, info->ai_socktype, info->ai_protocol);
}

bool Vun::Helpers::Sockets::Bind(SOCKETDESC socketfd, sockaddr* ainfo)
{
	if (bind(socketfd, ainfo, sizeof ainfo) == -1)
		return false;
	return true;
}
