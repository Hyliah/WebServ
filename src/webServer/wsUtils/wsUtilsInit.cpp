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
		pfd.events = POLLIN;
		pfd.revents = 0;
		_pollFds.push_back(pfd);	
	}
}

// // original
void WebServer::checkTimeouts() {
    for (size_t i = 0; i < _pollFds.size(); ) {

        int fd = _pollFds[i].fd;

        if (!isServerFd(fd)) {
            std::map<int, SocketClient*>::iterator it = _socketClients.find(fd);
            if (it != _socketClients.end()) {

                SocketClient* client = it->second;
                if (client && isTimedOut(client)) {
					client->state = ERROR;
					client->errorCode = 400;
					throw ResponseException(fd, 400);
                    // closeConnection(fd); //je crois q
                    // continue;
                }
            }
        }
        i++;
    }
}

// modif hygie 16.06
// void WebServer::checkTimeouts() {
//     for (size_t i = 0; i < _pollFds.size(); ) {

//         int fd = _pollFds[i].fd;

//         if (!isServerFd(fd)) {
//             std::map<int, SocketClient*>::iterator it = _socketClients.find(fd);
//             if (it != _socketClients.end()) {

//                 SocketClient* client = it->second;
//                 if (client && isTimedOut(client)) {
//                     closeConnection(fd);
// 					std::cout << std::endl;
//                     continue;
//                 }
//             }
//         }
//         i++;
//     }
// }

// // new version G 
// void WebServer::checkTimeouts() {
//     for (size_t i = 0; i < _pollFds.size(); ) {
//         int fd = _pollFds[i].fd;

//         if (!isServerFd(fd)) {
//             std::map<int, SocketClient*>::iterator it = _socketClients.find(fd);
//             if (it != _socketClients.end()) {
//                 SocketClient* client = it->second;
                
//                 if (client && isTimedOut(client)) {
//                     // Optionnel mais fortement recommandé pour éviter le statut 'pending' au client :
//                     std::string timeoutResponse = "HTTP/1.1 408 Request Timeout\r\nConnection: close\r\n\r\n";
//                     send(fd, timeoutResponse.c_str(), timeoutResponse.length(), 0);

//                     closeConnection(fd);
//                     // /!\ PIÈGE : closeConnection a supprimé l'élément à l'index i.
//                     // Le vecteur a shrink. Le prochain élément est DEJA à l'index i.
//                     // Donc on ne fait PAS i++, et on ne fait pas de continue inutile.
//                     continue; 
//                 }
//             }
//         }
//         i++; // On n'incrémente que si on n'a rien supprimé !
//     }
// }


bool WebServer::isTimedOut(const SocketClient* client) const
{
    const int TIMEOUT = 5;

    time_t now = time(NULL);

    double diff = difftime(now, client->lastActivity);

    LOG("TIMEOUT CHECK fd=" << client->getFd()
        << " now=" << now
        << " last=" << client->lastActivity
        << " diff=" << diff);

    return diff >= TIMEOUT;
}

// bool WebServer::isTimedOut(const SocketClient* client) const {
//     const int TIMEOUT = 5; // secondes (à adapter)

//     time_t now = time(NULL);

//     return (difftime(now, client->lastActivity) > TIMEOUT);
// }