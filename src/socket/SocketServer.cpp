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
	#include "Exceptions.hpp"
	#include <iostream>
	
/* ************************************************** */
/* construtor & destructors                           */
/* ************************************************** */

SocketServer::SocketServer() : _sockFd(-1), _res(NULL), _port("default"){
	createSocket();
}
SocketServer::SocketServer(std::string port) : _sockFd(-1), _res(NULL), _port(port){
	createSocket();
}
SocketServer::~SocketServer(){
	std::cout << "socket server destructeur  " << std::endl;
	if (_sockFd != -1)
		close(_sockFd);
	if (_res)
		freeaddrinfo(_res);
	std::cout << "close et free ok  " << std::endl;
}

/* ************************************************** */
/* getters & setters.                                 */
/* ************************************************** */

int									SocketServer::getFd() const{ return (_sockFd); }
const std::string&					SocketServer::getPort() const{ return (_port); }
const std::vector< const ServerConfig*>&	SocketServer::getServers() const{ return (_servers); }
struct addrinfo* 					SocketServer::getAddrinfo() const { return _res;}

void 								SocketServer::addServer(ServerConfig* server){ _servers.push_back(server); }


/* ************************************************** */
/* socket handling functions                          */
/* ************************************************** */

static bool setSocket(int sockFd);

//fucntions
void    SocketServer::createSocket(){
	struct addrinfo hints;

	if (initStructGetaddrinfo(hints, &_res) != 0)
		throw SocketException("Error : socket not created");

	_sockFd = socket(_res->ai_family, _res->ai_socktype, _res->ai_protocol);
	if (_sockFd == -1)
		throw SocketException("Error : socket not created");
	std::cout << "\nLE SOCKET A CE SOCKFD " << _sockFd << "\n" << std::endl;
	if (!setSocket(_sockFd))
		throw SocketException("Error : socket settings failed");

	if (bind(_sockFd, _res->ai_addr, _res->ai_addrlen) == -1)
		throw SocketException("Error : bind has failed");
}

	int SocketServer::initStructGetaddrinfo(struct addrinfo& hints, struct addrinfo** res){
		memset(&hints, 0, sizeof(hints));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_flags = AI_PASSIVE;

		int ret = getaddrinfo(NULL, _port.c_str(), &hints, res);
		
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


/* ************************************************** */
/* je ferai qu on y sera.                             */
/* ************************************************** */

void    SocketServer::listenSocket(int backlog){
	(void)backlog;
	//listen(fd, SOMAXCONN);
}

int     SocketServer::acceptClient(){
	return 1;
}

void    SocketServer::closeSocket(){
	
}
