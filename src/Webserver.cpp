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
#include <signal.h>

/* ************************************************** */
/* construtor & destructors                           */
/* ************************************************** */

// ici tu initialises tes sockets à partir de _servers
// for each server → créer un SocketServer sur le bon port
WebServer::WebServer(const std::vector<ServerConfig> &servers) : _servers(servers), _running(true)
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

/* ************************************************** */
/* PAUL LOOP			                              */
/* ************************************************** */

void	WebServer::pollLoop(){
	initPollStruct();
	
	while (_running){

		int ret = poll(&_pollFds[0], _pollFds.size(), 1000); //timeout de 1seconde 
		
		if (ret == -1)
			throw RunningException("Poll");
		else if (ret == 0) 
			continue;
		else  {
			for (int i = 0; i < _pollFds.size(); ++i){
				if (_pollFds[i].revents & POLLIN){ 
					int fd = _pollFds[i].fd;
					if (isServerFd(fd))
						acceptClient(fd);
					else
						handleRequest(fd);
				}
				if (_pollFds[i].revents & POLLOUT){
					int fd = _pollFds[i].fd;
					sendResponse(fd); 
				}
			}
		}
	}
}

void	WebServer::initPollStruct(){
	for (int i = 0; i < _socketServers.size(); ++i){
		int fd = _socketServers[i]->getFd();

		struct pollfd pfd;
		pfd.fd = fd;
		pfd.events = POLLIN;
		pfd.revents = 0;
		_pollFds.push_back(pfd);	
	}
}

void	WebServer::handleRequest(int fd){
	(void)fd;
}

void	WebServer::acceptClient(int fd){
	(void)fd;

	// accpt()
	//fncltdsdcadg
	// create new struct pollfds avec infos fd POLLIN 0
	// push
}

void	WebServer::sendResponse(int fd){
	(void)fd;
}

bool	WebServer::isServerFd(int fd){
	for (int i = 0; i < _socketServers.size(); ++i){
		if (_socketServers[i]->getFd() == fd)
			return true;
	}
	return false;
}






















/* DECHETS
	std::vector<ServerConfig>::iterator it;
	for (it = _servers.begin(); it != _servers.end(); ++it){
		_socketServers.push_back(SocketServer(it->port));
	}
*/