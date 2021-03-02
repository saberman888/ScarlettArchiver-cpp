#pragma once

#if defined(_WIN32)
#include <WinSock2.h>
#include <WS2tcpip.h>
#else
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#endif

#include <cstdlib>
#include <cstring>
#include <string>
#include <optional>
#include <type_traits>
#include <typeinfo>
#include <utility>

#ifdef _WIN32
using SOCKETDESC = SOCKET;
#else
using SOCKETDESC = int;
#endif

namespace Vun::Helpers::Sockets
{ 
	struct addrinfo* GetAddressInfo(const struct addrinfo* hints, std::optional<std::string> domain, int port = 80);
	SOCKETDESC InitSocket(struct addrinfo* info);
	bool Bind(SOCKETDESC socketfd, struct sockaddr* ainfo);
};
