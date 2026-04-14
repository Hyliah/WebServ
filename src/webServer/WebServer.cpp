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

	LOG("---- NEW POLL LOOP ----");

	while (_running){

		int ret = poll(&_pollFds[0], _pollFds.size(), 10000); //timeout de 1seconde ou 10 ?? changer peut etre
		LOG("poll() ret = " << ret);

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

				LOG("Checking fd: " << _pollFds[i].fd  << " revents: " << _pollFds[i].revents);
				
				if (_pollFds[i].revents & (POLLHUP | POLLERR)) {
    				closeConnection(_pollFds[i].fd);
    				continue;
				}
				
				if (_pollFds[i].revents & POLLIN){
					int fd = _pollFds[i].fd;
					LOG("POLLIN on fd " << fd); 

					if (isServerFd(fd)){
						try { acceptClient(fd); }
						catch (const ResponseException& e) {
    						sendResponse(e.getFd(), e.getCode());
						}
					}
					else {
						try { handleRequest(fd); }
						catch (const ResponseException& e) {
							sendResponse(e.getFd(), e.getCode());
						}
					}
				}
				if (_pollFds[i].revents & POLLOUT){
					int fd = _pollFds[i].fd;

					LOG("POLLOUT on fd " << fd);
					sendResponse(fd, 0);
					// delete de la boucle de paul
				}
			
			}
		}
	//on doit clode le fd du accept ?
	}
}

void	WebServer::acceptClient(int serverFd){

	LOG(">>> ACCEPT CLIENT on server fd " << serverFd);


	struct sockaddr_storage addr;
	socklen_t addrlen = sizeof(addr);
	int clientFd = accept(serverFd, (struct sockaddr*)&addr, &addrlen);
	if (clientFd < 0)
		throw ResponseException(clientFd, 500);

	int flags = fcntl(clientFd, F_GETFL, 0);
	if (fcntl(clientFd, F_SETFL, flags | O_NONBLOCK) < 0)
		throw ResponseException(clientFd, 500);
	

	SocketServer* serverPtr = NULL;
    for (size_t i = 0; i < _socketServers.size(); ++i) {
        if (_socketServers[i]->getFd() == serverFd) {
            serverPtr = _socketServers[i];
            break;
        }
    }
    if (!serverPtr)
        throw ResponseException(clientFd, 500);

	struct pollfd pfd;
	pfd.fd = clientFd;

	LOG("New client fd = " << clientFd);


	pfd.events = POLLIN;
	pfd.revents = 0;
	_pollFds.push_back(pfd);

	SocketClient* client = new SocketClient(clientFd, addr, serverPtr);
    _socketClients[clientFd] = client;
}

void	WebServer::handleRequest(int fd){
	
	LOG(">>> handleRequest fd = " << fd);

	char buffer[4096]; //4KB -->> mettre dans un fichier ?????????????????
	ssize_t bytes = recv(fd, buffer, sizeof(buffer), 0);
	LOG("recv bytes = " << bytes);

	if (bytes > 0) {
		SocketClient* client = _socketClients[fd];
		client->appendBuffer(std::string(buffer, bytes));

		LOG("BUFFER NOW:\n" << client->getBuffer());


		// if (client->getBuffer().size() > MAX_REQUEST_SIZE) {
		// 	return throw ResponseException(fd, 500);
		// }

		if (isHeaderComplete(client)) {
			
			LOG(">>> HEADER COMPLETE");
			if (!client->headerParsed){
				client->parseRequest();
				client->defineBodyType();
				client->cleanBuffer();
				// if (client->getRequest().getContentLength() > maxBodySize) //gestion du maxBodysize
				// 	throw ResponseException(fd, 413);
				
				// faire une fonction pour vider le buffer jusqu a rnrn -- e le laisse pcq je sais pas si c est fait
				client->headerParsed = true;

				LOG("Method: " << client->getRequest().getMethod());
				LOG("URI: " << client->getRequest().getUri());

			}
			
			if (client->getRequest().getMethod() == "DELETE" || client->getRequest().getMethod() == "GET")
				client->ignoreBody = true;
			
			

			if (client->chunked)
				client->parsingChunked();
			else if (client->contentLength) 
				client->parsingContentLength();	
			else
				client->parsingNoBody();

			LOG("Chunked: " << client->chunked);
			LOG("ContentLength: " << client->contentLength);

			if (client->requestCompleted){

				LOG(">>> REQUEST COMPLETE");
				
				setPollOut(fd);
			}
		}
		else
			LOG("Header NOT complete yet");
	}

	else if (bytes == 0) {
		closeConnection(fd);
	}

	else {
		if (errno != EAGAIN && errno != EWOULDBLOCK)
			closeConnection(fd);
		// else {
		// 	throw ResponseException(fd, 404);
		// }
	}
}
/*
if (client.buffer.size() > MAX_REQUEST_SIZE)
	→ 413 Payload Too Large

*/

void	WebServer::sendResponse(int fd, int codeError){

	LOG(">>> sendResponse fd = " << fd);


	SocketClient* client = _socketClients[fd];
	
	std::string method = client->getRequest().getMethod();

	LOG("Method = " << method);

	HttpResponse res;

	if (codeError != 0){
		res = buildErrorResponse(codeError);
	}

	else {	
		if (method == "GET")
		//mettre le try and catch et recup le fd etcode derreur pour faire un res d errreur
			res = methodGet(client);
		else if (method == "POST")
			res = methodPost(); //mettre en res mais ce sera pour plus tard
		else if (method == "DELETE")
			res = methodDelete(); //mettre en res mais ce sera pour plus tard
	}
    std::string response = res.ResponseToString();

	LOG(">>> RESPONSE BUILT:");
	LOG(response);
	
	size_t totalSent = 0;
	while (totalSent < response.size()) {
		ssize_t sent = send(fd, response.c_str() + totalSent, response.size() - totalSent, 0);
		
		LOG("Bytes sent: " << sent);
			
		if (sent <= 0) {
			closeConnection(fd);
			return;
		}
		totalSent += sent;
	}

	closeConnection(fd);

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