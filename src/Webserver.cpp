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
#include <string>

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
	for (std::map<int, SocketClient*>::iterator it = _socketClients.begin();
		 it != _socketClients.end(); ++it) {
		
		close(it->first);          // ferme le fd
		delete it->second;         // delete le client
	}
	_socketClients.clear();

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
	SocketClient* client = new SocketClient(fd, addr);
	_socketClients[fd] = client;
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
		
		//std::cout << "_pollFds.size(): " << _pollFds.size() << std::endl;
		
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
			size_t size = _pollFds.size();
			for (size_t i = 0; i < size; ++i){
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
					// delete de la boucle de paul
				}
			}
		}
	}

	//on doit clode le fd du accept ?
}

/*
	headers + body == Content-Length
*/

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

void	WebServer::handleRequest(int fd){
	
	char buffer[4096]; //4KB
	ssize_t bytes = recv(fd, buffer, sizeof(buffer), 0);

	if (bytes > 0) {
		SocketClient* client = _socketClients[fd];
		client->appendBuffer(std::string(buffer, bytes));

		if (client->getBuffer().size() > MAX_REQUEST_SIZE) { // DEFINIR LE MAX_REQUEST_SIZE
			// faire une fonction erreur client.setError(413);
			//setPollOut(fd);
			return; //on enleve le paul de la struct mais le server reste
		}

		if (isHeaderComplete(client)) {
			if (!client->headerParsed){
				client->parseRequest();
				client->defineBodyType();
				client->cleanBuffer();
				//if (client.getContentLength() > maxBodySize)
					// ERROR 413
				
					// std::cout << "--- DEBUG BUFFER BODY BEGIN ---" << std::endl;
    				// std::cout << client->getBuffer().substr(0, 100) << "..." << std::endl; // Affiche les 100 premiers caractères
    				// std::cout << "--- DEBUG BUFFER BODY END ---" << std::endl;
				// faire une fonction pour vider le buffer jusqu a rnrn
				client->headerParsed = true;
			}
			
			if (client->getRequest().getMethod() == "DELETE" || client->getRequest().getMethod() == "GET")
				client->ignoreBody = true;
			

			if (client->chunked)
				client->parsingChunked();
			else if (client->contentLength) 
				client->parsingContentLength();	
			else
				client->parsingNoBody();

			if (client->requestCompleted)
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
/*
if (client.buffer.size() > MAX_REQUEST_SIZE)
	→ 413 Payload Too Large

*/

void	WebServer::sendResponse(int fd){
	std::string method = client.getMethod();

	if (method == "GET")
		methodGet();
	else if (method == "POST")
		methodPost();
	if (method == "DELETE")
		methodDelete();
	//else 
	//gros problem sa mere
}

/* ************************************************** */
/* UTILS RESPONSE.     		                          */
/* ************************************************** */

void	WebServer::methodGet(){
	/*
	- lire le fichier ?httprequest
	- générer le body 
	- creation d une http response -> stringstream 
	- ajout des headers
	- fonction send : send(fd, response.c_str(), response.size(), 0);
	- closeConnection(fd);
	*/
}

void	WebServer::methodPost(){

}

void	WebServer::methodDelete(){

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























/* DECHETS
	std::vector<ServerConfig>::iterator it;
	for (it = _servers.begin(); it != _servers.end(); ++it){
		_socketServers.push_back(SocketServer(it->port));
	}
*/










/*
HARDCODE DU SENDRESPONSE 

// std::string toString(size_t n) {
//     std::stringstream ss;
//     ss << n;
//     return ss.str();
// }

// void WebServer::sendResponse(int fd) {
//     std::cout << "[SEND] réponse envoyée fd=" << fd << std::endl;
    
//     std::string body = "<html><body><h1>Ca marche</h1></body></html>";
    
//     // On construit la réponse proprement
//     std::stringstream ss;
//     ss << "HTTP/1.1 200 OK\r\n";
//     ss << "Content-Type: text/html\r\n";
//     ss << "Content-Length: " << body.size() << "\r\n"; // TRÈS IMPORTANT
//     ss << "Connection: close\r\n";
//     ss << "\r\n"; // La ligne vide qui sépare les headers du body
//     ss << body;
    
//     std::string response = ss.str();
//     send(fd, response.c_str(), response.size(), 0);
    
//     // On ferme après le send car on a mis "Connection: close"
//     closeConnection(fd);
// }

*/