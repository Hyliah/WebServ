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

		LOG("SERVER FD ADDED: " << fd);

		struct pollfd pfd;
		pfd.fd = fd;
		pfd.events = POLLIN; //changement ici
		pfd.revents = 0;
		_pollFds.push_back(pfd);	
	}
}

void WebServer::checkTimeouts() {
    for (size_t i = 0; i < _pollFds.size(); ) {

        int fd = _pollFds[i].fd;

        if (!isServerFd(fd)) {
            SocketClient* client = _socketClients[fd];

            if (client && isTimedOut(client)) {
                LOG("Timeout client fd = " << fd);
                closeConnection(fd);
                continue;
            }
        }

        i++;
    }
}

bool WebServer::isTimedOut(const SocketClient* client) const {
    if (!client)
        return false;

    const int TIMEOUT = 30; // secondes (à adapter)

    time_t now = time(NULL);

    return (difftime(now, client->lastActivity) > TIMEOUT);
}