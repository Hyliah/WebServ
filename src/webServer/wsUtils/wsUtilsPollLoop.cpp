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


bool	WebServer::isServerFd(int fd){
	for (size_t i = 0; i < _socketServers.size(); ++i){
		if (_socketServers[i]->getFd() == fd)
			return true;
	}
	return false;
}

bool 	WebServer::isHeaderComplete(SocketClient* client){
	const std::string& buffer = client->getBuffer();
	size_t pos = buffer.find("\r\n\r\n");

	if (pos == std::string::npos)
		return false; // ca veut dire que le rnrn n est encore dans le recv -> requete pas terminée
	return true;
}

void	WebServer::removePollFd(int fd){
	std::vector<struct pollfd>::iterator it;

	for (it = _pollFds.begin(); it != _pollFds.end(); ++it){
		if (it->fd == fd){
			_pollFds.erase(it);
			break;
		}
	}
}

void WebServer::closeConnection(int fd){
	
	LOG("Closing connection fd = " << fd);

	if (_socketClients.find(fd) == _socketClients.end()) {
		LOG("⚠️ FD NOT FOUND (double close?)");
	}
	
	close(fd);

	std::map<int, SocketClient*>::iterator it = _socketClients.find(fd);
	if (it != _socketClients.end()) {
		delete it->second;
		_socketClients.erase(it);
	}

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

	LOG("Switching fd " << fd << " to POLLOUT");
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

-> merci chatos
if (_pollFds[i].revents & (POLLERR | POLLHUP)) {
	closeConnection(fd);
	continue;
}


POLLHUP
Le client a fermé la connexion

POLLERR
Erreur sur le socket


*/