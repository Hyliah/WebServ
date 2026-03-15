/*****************************************************************************/
/*                                                                           */
/*                                                                           */
/*                        LES CODEUSES DU DIMANCHE                           */
/*                                FONT UN                                    */
/*                                                                           */
/*                   _ _ _ ____ ___  ____ ____ ____ _  _                     */
/*                   | | | |___ |__] [__  |___ |__/ |  |                     */
/*                   |_|_| |___ |__] ___] |___ |  \  \/                      */
/*                                                                           */
/*****************************************************************************/

	#include "SocketServer.hpp"
	
	
// construtor & destructors
SocketServer::SocketServer() : _sockFd(-1), _port("default"), _servers(NULL), _res(NULL){}
SocketServer::SocketServer(int fd) : _sockFd(fd), _port("default"), _servers(NULL), _res(NULL){}
SocketServer::~SocketServer(){
	if (_sockFd != -1)
		close(_sockFd);
	if (_res)
		freeaddrinfo(_res);
}

// getters and setters
int									SocketServer::getFd() const{ return (_sockFd); }
const std::string&					SocketServer::getPort() const{ return (_port); }
const std::vector<ServerConfig*>&	SocketServer::getServers() const{ return (_servers); }
struct addrinfo* 					SocketServer::getAddrinfo() const { return _res;}

void 								SocketServer::setPort(const std::string& port){ _port = port; }
void 								SocketServer::addServer(ServerConfig* server){ _servers.push_back(server); }



// socket handling functions
static int initStructGetaddrinfo(struct addrinfo& hints, struct addrinfo** res);
static bool setSocket(int sockFd);
void    SocketServer::createSocket(){
	struct addrinfo hints;

	if (initStructGetaddrinfo(hints, &_res) != 0)
		throw SocketException("Error : socket not created");

	_sockFd = socket(_res->ai_family, _res->ai_socktype, _res->ai_protocol);
	if (_sockFd == -1)
		throw SocketException("Error : socket not created");

	if (!setSocket(_sockFd))
		throw SocketException("Error : socket settings failed");

	if (bind(_sockFd, _res->ai_addr, _res->ai_addrlen) == -1)
		throw SocketException("Error : bind has failed");
}

	static int initStructGetaddrinfo(struct addrinfo& hints, struct addrinfo** res){
		memset(&hints, 0, sizeof(hints));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_flags = AI_PASSIVE;

		int ret = getaddrinfo(NULL, "8080", &hints, res);
		
		return ret;
	}

	static bool setSocket(int sockFd){
		int opt = 1;
		if (setsockopt(sockFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
			return false;

		int flags = fcntl(sockFd, F_GETFL, 0);
		if (flags < 1)
			return false;
		if (fcntl(sockFd, F_SETFL, flags | O_NONBLOCK) < 0)
			return false;

		return true;
	}


//je les ferai quand on y sera
void    SocketServer::listenSocket(int backlog){
	
	//listen(fd, SOMAXCONN);
}

int     SocketServer::acceptClient(){
	
}

void    SocketServer::closeSocket(){
	
}
