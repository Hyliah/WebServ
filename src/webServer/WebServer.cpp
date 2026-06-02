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
		
		close(it->first);
		delete it->second;
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

void WebServer::pollLoop() {
    initPollStruct();
    LOG("\n---- NEW POLL LOOP ----"); //------------------------------------------

    while (_running) {

        int ret = poll(&_pollFds[0], _pollFds.size(), 1000);

        if (ret == -1) {
            if (errno == EINTR) {
                _running = false;
                break;
            } else {
                throw RunningException(std::string("Poll: ") + strerror(errno));
            }
        }

        //checkTimeouts();
		try {
            checkTimeouts();
        } catch (const ResponseException& e) {
        	sendResponse(e.getFd());
        }

        if (ret == 0)
            continue;

        for (size_t i = 0; i < _pollFds.size(); ) {

            int fd = _pollFds[i].fd;
            short revents = _pollFds[i].revents;

            if (revents & POLLIN) {

                if (isServerFd(fd)) {
                    try {
                        acceptClient(fd);
                    } catch (const ResponseException& e) {
                        sendResponse(e.getFd());
                    }
                } else {
                    try {
                        handleRequest(fd);
                    } catch (const ResponseException& e) {
                        sendResponse(e.getFd());
                    }
                }
            }

            if (revents & POLLOUT) {

                try {
                    sendResponse(fd);
                } catch (...) {
                    closeConnection(fd);
					// 500
                    continue;
                }
            }

			if (revents & (POLLHUP | POLLERR)) {

				SocketClient* client = _socketClients[fd];

				if (client && client->state == PROCESSING) {
					setPollOut(fd);
					i++;
					continue;
				}
				
				closeConnection(fd);
                continue;
            }
            i++;
        }
    }
}

void	WebServer::acceptClient(int serverFd){
	LOG("\n >>> ACCEPT CLIENT on server fd " << serverFd); // ---------------------

	struct sockaddr_storage addr;
	socklen_t addrlen = sizeof(addr);
	int clientFd = accept(serverFd, (struct sockaddr*)&addr, &addrlen);
	if (clientFd < 0)
		return ;
	
	if (_socketClients.size() >= 1024) {
		LOG("Too many clients");
		std::string res =
			"HTTP/1.1 503 Service Unavailable\r\n"
			"Content-Length: 0\r\n"
			"Connection: close\r\n\r\n";

		send(clientFd, res.c_str(), res.size(), 0);
		close(clientFd);
		return;
	}

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
    if (!serverPtr){
        throw ResponseException(clientFd, 500);
	}
	
	SocketClient* client = new SocketClient(clientFd, addr, serverPtr);
	
	client->lastActivity = std::time(NULL);

    _socketClients[clientFd] = client;

	struct pollfd pfd;
	pfd.fd = clientFd;
	pfd.events = POLLIN;
	pfd.revents = 0;
	_pollFds.push_back(pfd);
	
	LOG("New client fd = " << clientFd); // ------------------------------------
}

void	WebServer::handleRequest(int fd){
	LOG("\n>>> HANDLE REQUEST "); // -------------------------------------------
	LOG(">>> handleRequest fd = " << fd); // -----------------------------------
	SocketClient* client = _socketClients[fd];

	try {
		char buffer[4096];
		ssize_t bytes = recv(fd, buffer, sizeof(buffer), 0);
		
		if (bytes == 0) {
			closeConnection(fd);
			return ;
		}
		
		if (bytes < 0) {
			if (errno == EAGAIN || errno == EWOULDBLOCK)
        		return; // est ce que on a droit au errno ici ? 
    		closeConnection(fd);
			//return 500
			return ;
		}
		
		client->appendBuffer(std::string(buffer, bytes));
		client->lastActivity = std::time(NULL);
		
		if (client->state == READING)
            client->parseRequest();
		
		if (client->state == BODY_READING)
            parseBody(client);
		
		if (client->state == READY) {
            client->state = PROCESSING;
            setPollOut(fd);
        }
	}
    catch (const ResponseException& e)
    {
        client->errorCode = e.getCode();
		LOG("error code : " << client->errorCode);
        client->state = ERROR;
        setPollOut(fd);
    }
}

void WebServer::parseBody(SocketClient* client){
	LOG("\n>>> PARSE BODY "); // -------------------------------------------------------------

	client->lastActivity = std::time(NULL);

	if (client->getRequest().getMethod() == "DELETE" || client->getRequest().getMethod() == "GET")
		client->ignoreBody = true;
    
	if (client->chunked){
        client->parsingChunked();
	}
    
	else if (client->contentLength){
        client->parsingContentLength();
	}

    else{
        client->parsingNoBody();
	}

    if (client->requestCompleted)
        client->state = READY;
}

void	WebServer::sendResponse(int fd){
	LOG("\n>>> SEND RESPONSE "); // -------------------------------------------------------------
	SocketClient* client = _socketClients[fd];
	
	try {
		HttpResponse res;
		const LocationConfig* location = findMatchingLocation(client);

		if (client->state != ERROR){
			try{ resolvePath(client); }
			catch (const ResponseException& e) {
				client->state = ERROR;
				client->errorCode = e.getCode();
			}
		}
		
		if (client->state == ERROR) {
			res = buildErrorResponse(client->errorCode, client); 
		}
		
		else if (!location){
			res = buildErrorResponse(404, client); }

		else if (location->returnCode){
			res = buildRedirectResponse(location->returnCode, location->returnUrl, client); }

		else {

			std::string method = client->getRequest().getMethod();

			LOG("la methose est : " << method);
			

			if (method == "GET"){
				res = methodGet(client, location);
			}
            else if (method == "POST")
                res = methodPost(client, location);
            else if (method == "DELETE")
                res = methodDelete(client, location);
		}

		std::string response = res.ResponseToString();

		size_t totalSent = 0;
		while (totalSent < response.size()) {
			ssize_t sent = send(fd, response.c_str() + totalSent, response.size() - totalSent, 0);
			
			LOG("Bytes sent: " << sent);
				
			if (sent <= 0 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
				closeConnection(fd);
				return;
			}
			totalSent += sent;
		}
		
		client->state = DONE;
		closeConnection(fd);
	}
	    catch (...) {
		// response 500
        closeConnection(fd);
    }
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