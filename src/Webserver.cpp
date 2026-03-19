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

#include "WebServer.hpp"
#include <iterator>
#include <map>

/* ************************************************** */
/* construtor & destructors                           */
/* ************************************************** */

// ici tu initialises tes sockets à partir de _servers
// for each server → créer un SocketServer sur le bon port
WebServer::WebServer(const std::vector<ServerConfig> &servers) : _servers(servers)
{
	initSockets();
	cpyLinkConfig();
}

WebServer::~WebServer(){
	for (std::vector<SocketServer*>::iterator it = _socketServers.begin();
		it != _socketServers.end(); ++it) {
	delete *it;
    }
    _socketServers.clear();
}

/* ************************************************** */
/* getters & setters.                                 */
/* ************************************************** */

// SocketServer&	WebServer::getServer(size_t idx){}
// SocketClient&	WebServer::getClient(int fd){}
// void	WebServer::setSocket(SocketServer& socket){}
// void 	WebServer::setClient(int fd, SocketClient& client){}
// // or
// void 	WebServer::addClient(int fd){} // faire la construction du Client directement dans cette fonction




/* ************************************************** */
/* socket intit functions                             */
/* ************************************************** */

//void	WebServer::removeClient(int fd){}

void	WebServer::initSockets(){

	std::vector<ServerConfig>::iterator it;

	for (it = _servers.begin(); it != _servers.end(); ++it){	
		
		bool exists = false;
		for (size_t i = 0; i < _socketServers.size(); ++i) {
			if (_socketServers[i]->getPort() == it->port) {
				exists = true;
				break;
			}
		}
		if (!exists) {
			_socketServers.push_back(new SocketServer(it->port));
		}
	}
}

void	WebServer::cpyLinkConfig(){

	std::vector<ServerConfig>::iterator it;

	for (it = _servers.begin(); it != _servers.end(); ++it){	
		std::vector<SocketServer*>::iterator itserv;
		for (itserv = _socketServers.begin(); itserv != _socketServers.end(); ++itserv)
		{
			if ((*itserv)->getPort() == it->port){
				(*itserv)->addServer(&(*it));
				break;
			}
		}
	}	
}

// faire une boucle while qui va créer tous les socket. 

void WebServer::handleRequest(SocketClient& client){
	(void)client;
} //fonction qui va démarrer le parsing du http dans la classe SocketClient
//paul loop ?***







/* DECHETS
	std::vector<ServerConfig>::iterator it;
	for (it = _servers.begin(); it != _servers.end(); ++it){
		_socketServers.push_back(SocketServer(it->port));
	}
*/