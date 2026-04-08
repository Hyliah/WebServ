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

/* ************************************************** */
/* construtor & destructors                           */
/* ************************************************** */

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

// void	WebServer::addClient(int fd, struct sockaddr_storage addr){
// 	SocketClient* client = new SocketClient(fd, addr);
// 	_socketClients[fd] = client;
// }


/* ************************************************** */
/* PAUL LOOP			                              */
/* ************************************************** */

void	WebServer::pollLoop(){
	initPollStruct();
	
	while (_running){

		int ret = poll(&_pollFds[0], _pollFds.size(), 10000); //timeout de 1seconde ou 10 ?? changer peut etre
		
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

void	WebServer::acceptClient(int serverFd){
	struct sockaddr_storage addr;
	socklen_t addrlen = sizeof(addr);
	int clientFd = accept(serverFd, (struct sockaddr*)&addr, &addrlen);
	if (clientFd < 0)
		throw RunningException("accept");

	int flags = fcntl(clientFd, F_GETFL, 0);
	if (fcntl(clientFd, F_SETFL, flags | O_NONBLOCK) < 0)
		throw RunningException("fcntl");
	

	SocketServer* serverPtr = NULL;
    for (size_t i = 0; i < _socketServers.size(); ++i) {
        if (_socketServers[i]->getFd() == serverFd) {
            serverPtr = _socketServers[i];
            break;
        }
    }
    if (!serverPtr)
        throw RunningException("acceptClient: serverFd not found");

	struct pollfd pfd;
	pfd.fd = clientFd;
	pfd.events = POLLIN;
	pfd.revents = 0;
	_pollFds.push_back(pfd);

	SocketClient* client = new SocketClient(clientFd, addr, serverPtr);
    _socketClients[clientFd] = client;
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































// supprimer si ca compile bien :)


/*
// std::string	WebServer::resolvePath(){
// 	std::string& uri = client._request.getUri();
	
// 	size_t pos = uri.find('?');
// 	std::string path = uri.substr(0, pos);
// 	std::string finalPath;
// 	//std::string query = uri.substr(pos, uri.end()); -> si on veut gerer ca

// 	finalPath = decodePath(path); // gestion avec des if catch throw and shit
// 	finalPath = normalizePath(finalPath); //same
// 	checkErrorPath();

// 	const std::vector<const ServerConfig*>& conf = client.getServer().getServers();

// 	HttpRequest& req = client.getRequest();
// 	const std::map<std::string, std::string>& headers = req.getHeaders();

// 	std::string host;
// 	std::map<std::string,std::string>::const_iterator it = headers.find("host");
// 	if (it != headers.end())
// 		host = it->second;
// 	else
// 		host = ""; // ou fallback
	

// 	std::string recupRoot;
// 	for (size_t i = 0; i < conf.size(); ++i) {
// 		const ServerConfig* cfg = conf[i];
// 		if (cfg->serverName == host) {
// 			recupRoot = cfg->root;
// 			break;
// 		}
// 	}

// 	// fallback sur le premier serveur si host pas trouvé
// 	if (recupRoot.empty() && !conf.empty())
// 		recupRoot = conf[0]->root;

// 	finalPath = recupRoot + finalPath;

// 	return finalPath;

// }
*/