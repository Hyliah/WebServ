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

#ifndef WEBSERVER_HPP
#define WEBSERVER_HPP

#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "SocketClient.hpp"
#include "SocketServer.hpp"
#include "Exceptions.hpp"

#define MAX_REQUEST_SIZE 8192

class WebServer{

	private :
		
		std::vector<SocketServer*>	    _socketServers;
		std::map<int, SocketClient*>	_socketClients;
		std::vector<ServerConfig>	    _servers;
		std::vector<struct pollfd>	    _pollFds; //pauls 
        
		WebServer(const WebServer &other);
		WebServer& operator=(const WebServer &other);
		
	public :

		WebServer(const std::vector<ServerConfig> &servers);
		~WebServer();
	
		bool	_running;
		
		void	cpyLinkConfig();
		void	initSockets();
		void	initPollStruct();

		
		//gettes & setters
		// SocketServer&	getServer(size_t idx);
		// SocketClient&	getClient(int fd);
		// void				setSocket(SocketServer& socket);
		// void 			setClient(int fd, SocketClient& client);
		void 	addClient(int fd, struct sockaddr_storage addr);
		
		// autres
		//void	removeClient(int fd);
		
		//PAUL LOOP
		void	pollLoop();
		void	acceptClient(int fd);
		void	handleRequest(int fd);
		void	sendResponse(int fd);
		
		//PAUL LOOP UTILS
		bool	isServerFd(int fd);
		bool 	isHeaderComplete(SocketClient* client);
		
		void	closeConnection(int fd);
		void	removePollFd(int fd);
		void	setPollOut(int fd);
};

#endif









































	// 	////////// TTTEEEEEEEESTTTTT

	// 	void testPrintSocket() const {
    // std::cout << "=== WebServer SocketServers ===" << std::endl;

    // int socketCount = 0;

    // for (std::vector<SocketServer*>::const_iterator sockIt = _socketServers.begin();
    //      sockIt != _socketServers.end(); ++sockIt) {
    //     const SocketServer* sock = *sockIt;

    //     std::cout << "\n[" << ++socketCount << "] SocketServer" << std::endl;
    //     std::cout << "  Port: " << sock->getPort() << std::endl;
    //     std::cout << "  Socket FD: " << sock->getFd() << std::endl;

    //     const std::vector<const ServerConfig*>& servers = sock->getServers();
    //     if (servers.empty()) {
    //         std::cout << "  No ServerConfig attached!" << std::endl;
    //         continue;
    //     }

    //     int serverCount = 0;
    //     for (std::vector<const ServerConfig*>::const_iterator servIt = servers.begin();
    //          servIt != servers.end(); ++servIt) {
    //         const ServerConfig* server = *servIt;

    //         std::cout << "  [" << ++serverCount << "] ServerConfig" << std::endl;
    //         std::cout << "    MaxBodySize: " << server->maxBodySize << std::endl;

    //         const std::vector<std::string>& index = server->index;
    //         std::cout << "    Index files:";
    //         if (index.empty()) std::cout << " (none)" << std::endl;
    //         else {
    //             std::cout << std::endl;
    //             for (std::vector<std::string>::const_iterator idxIt = index.begin();
    //                  idxIt != index.end(); ++idxIt) {
    //                 std::cout << "      - " << *idxIt << std::endl;
    //             }
    //         }

    //         const std::map<int,std::string>& errors = server->errorPages;
    //         std::cout << "    Error pages:";
    //         if (errors.empty()) std::cout << " (none)" << std::endl;
    //         else {
    //             std::cout << std::endl;
    //             for (std::map<int,std::string>::const_iterator errIt = errors.begin();
    //                  errIt != errors.end(); ++errIt) {
    //                 std::cout << "      " << errIt->first << " -> " << errIt->second << std::endl;
    //             }
    //         }

    //         const std::vector<LocationConfig>& locations = server->locations;
    //         std::cout << "    Locations:";
    //         if (locations.empty()) std::cout << " (none)" << std::endl;
    //         else {
    //             std::cout << std::endl;
    //             int locCount = 0;
    //             for (std::vector<LocationConfig>::const_iterator locIt = locations.begin();
    //                  locIt != locations.end(); ++locIt) {
    //                 const LocationConfig& loc = *locIt;
    //                 std::cout << "      [" << ++locCount << "] Location" << std::endl;
    //                 std::cout << "        Path: " << loc.path << std::endl;
    //                 std::cout << "        Root: " << loc.root << std::endl;

    //                 const std::vector<std::string>& locIndex = loc.index;
    //                 std::cout << "        Index files:";
    //                 if (locIndex.empty()) std::cout << " (none)" << std::endl;
    //                 else {
    //                     std::cout << std::endl;
    //                     for (std::vector<std::string>::const_iterator idxIt2 = locIndex.begin();
    //                          idxIt2 != locIndex.end(); ++idxIt2) {
    //                         std::cout << "          - " << *idxIt2 << std::endl;
    //                     }
    //                 }

    //                 const std::vector<std::string>& methods = loc.methods;
    //                 std::cout << "        Allowed Methods:";
    //                 if (methods.empty()) std::cout << " (none)" << std::endl;
    //                 else {
    //                     std::cout << " ";
    //                     for (std::vector<std::string>::const_iterator mIt = methods.begin();
    //                          mIt != methods.end(); ++mIt) {
    //                         std::cout << *mIt << " ";
    //                     }
    //                     std::cout << std::endl;
    //                 }
    //             }
    //         }

    //     } // end ServerConfig loop
    // } // end SocketServer loop

    // std::cout << "\n=== End of WebServer Info ===" << std::endl;
	//}