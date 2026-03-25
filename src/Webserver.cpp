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
#include <poll.h>

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

void	WebServer::addClient(int fd, struct sockaddr_storage addr){
	SocketClient tmp(fd, addr);
	_socketClients[fd] = tmp;
}

/* ************************************************** */
/* socket & general intit functions                   */
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

void	WebServer::initPollStruct(){
	for (size_t i = 0; i < _socketServers.size(); ++i){
		int fd = _socketServers[i]->getFd();

		struct pollfd pfd;
		pfd.fd = fd;
		pfd.events = POLLIN; //changement ici
		pfd.revents = 0;
		_pollFds.push_back(pfd);	
	}
}

/* ************************************************** */
/* PAUL LOOP			                              */
/* ************************************************** */

void	WebServer::pollLoop(){
	initPollStruct();
	
	while (_running){

		int ret = poll(&_pollFds[0], _pollFds.size(), 10000); //timeout de 1seconde 
		
		std::cout << "_pollFds.size(): " << _pollFds.size() << std::endl;
		
		if (ret == -1) {
			if (errno == EINTR) {
				_running = false;
				break;
			} else 
				throw RunningException(std::string("Poll: ") + strerror(errno));
		}
		else if (ret == 0) 
			continue;
		else  {
			for (size_t i = 0; i < _pollFds.size(); ++i){
				// METTRE ICI LES POLLER ET POLLHUP
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

	//on doit clode le fd du accept ?
}

/*
	headers + body == Content-Length
*/

void	WebServer::handleRequest(int fd){

	char buffer[4096]; //4KB
	ssize_t bytes = recv(fd, buffer, sizeof(buffer), 0);

	if (bytes > 0) {
		SocketClient& client = _socketClients[fd];
		client.appendBuffer(std::string(buffer, bytes));

		//le buffer de la requete est rempli en tout cas jusqu'au rnrn
        if (isRequestComplete(client)) {
            
			client.parseRequest();
            client.handleLength();

			if ((client.chunked || client.contentLength) && (client.getMethod() == "DELETE" || client.getMethod() == "GET"))
				return; //BIG PROBLEM -> verif ca mais il me semble que oui, chatyy nous le dira
			
			// 3 gestion du parsing de body
			if (!client.chunked && !client.contentLength) 
				client.parsingNoBody();
			else if (client.chuked)
				client.parsingChunked();
			else
				client.parsingContentLength();


			setPollOut(fd);
        }
	}

	else if (bytes == 0) {
		closeConnection(fd);
	}

	else {
		if (errno != EAGAIN && errno != EWOULDBLOCK)
        	closeConnection(fd);
		// else {
		// 	throw ? de quoi on verra //404 ou un vrai throw
		// }
	}
}

void	WebServer::acceptClient(int serverFd){
	struct sockaddr_storage addr;
	socklen_t addrlen = sizeof(addr);
	int clientFd = accept(serverFd, (struct sockaddr*)&addr, &addrlen);
	if (clientFd < 0)
		throw RunningException("accept");

	int flags = fcntl(clientFd, F_GETFL, 0);
	if (fcntl(clientFd, F_SETFL, flags | O_NONBLOCK) < 0)
		throw RunningException("fcntl");
	
	struct pollfd pfd;
	pfd.fd = clientFd;
	pfd.events = POLLIN;
	pfd.revents = 0;
	_pollFds.push_back(pfd);
	
	addClient(clientFd, addr);
}

void	WebServer::sendResponse(int fd){
	(void)fd;
}

/* ************************************************** */
/* UTILS PAUL LOOP			                          */
/* ************************************************** */

bool	WebServer::isServerFd(int fd){
	for (size_t i = 0; i < _socketServers.size(); ++i){
		if (_socketServers[i]->getFd() == fd)
			return true;
	}
	return false;
}

bool 	WebServer::isRequestComplete(SocketClient& client){
	const std::string& buffer = client.getBuffer();
	size_t pos = buffer.find("\r\n\r\n");

	if (pos == npos)
		return false; // ca veut dire que le rnrn n est encore dans le recv -> requete pas terminée
	return true;
}

void	WebServer::removePollFd(int fd){
	// std::vector<struct pollfd>::iterator it;

	// for (it = _pollFds.begin(); it != _pollFds.end(); ++it){
	// 	if (it->fd == fd){
	// 		->>>>>>>>>>>>> REMOVE
	// 		break;
	// 	}
	// }
	(void)fd;
}

void	WebServer::closeConnection(int fd){
	close(fd); //utile si on met dans le destructeur ?????
	_socketClients.erase(fd);
	removePollFd(fd);
}

void	WebServer::setPollOut(int fd){
	std::vector<struct pollfd>::iterator it;

	for (it = _pollFds.begin(); it != _pollFds.end(); ++it){
		if (it->fd == fd){
			it->events = POLLOUT; // it->events = POLLOUT | POLLERR | POLLHUP;
			break;
		}
	}
}







// SI ON VEUT
/*
Meileure gestion des erreur des PAUL 
Mettre dans boucle paul au endroit indiqués les différents checks 
et changer dans l init en POLLIN + au setPollOut -> it->events = POLLOUT/POLLIN | POLLERR | POLLHUP;

if (revents & POLLERR) {
    closeConnection(fd);
}
else if (revents & POLLHUP) {
    closeConnection(fd);
}
else if (revents & POLLIN) {
    handleRequest(fd);
}
else if (revents & POLLOUT) {
    sendResponse(fd);
}



POLLHUP
Le client a fermé la connexion

POLLERR
Erreur sur le socket


*/























/* DECHETS
	std::vector<ServerConfig>::iterator it;
	for (it = _servers.begin(); it != _servers.end(); ++it){
		_socketServers.push_back(SocketServer(it->port));
	}
*/