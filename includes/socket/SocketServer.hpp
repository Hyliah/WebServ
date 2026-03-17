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

#ifndef SOCKETSERVER_HPP
#define SOCKETSERVER_HPP

#include <string>
#include <vector>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include "ServerConfig.hpp"

class SocketServer {

private:

    int                             _sockFd;
	struct addrinfo					*_res;
    std::string                     _port;
    std::vector<ServerConfig*>      _servers;

public:

    SocketServer();
	SocketServer(int fd);
	SocketServer(const SocketServer& other) = delete;
	SocketServer& operator=(const SocketServer& other) = delete;
    ~SocketServer();

    void    createSocket();
    void    listenSocket(int backlog = 10);

    int     acceptClient();

    void    closeSocket();

    int     getFd() const;


	struct addrinfo* getAddrinfo() const;

    const std::string& getPort() const;
    void setPort(const std::string& port);

    void addServer(ServerConfig* server);
    const std::vector<ServerConfig*>& getServers() const;
};

#endif