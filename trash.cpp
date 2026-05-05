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



// Ici on peut faire comme avant et stocker toutes les fonctions de test
// qu'on utilise plus mais pour les avoir en back up si jamais 

//VERSION TEST POUR LE PARSER CONFIG AVEC PLEIN DE COUT 

// VERSION TEST BELOW
// void ParserConfig::parse(const std::string &configFilePath) {
// 	// test readfile
// 	std::string content = readFile(configFilePath);
// 	if (content.empty()) {
// 		throw ParseException("Config file is empty: " + configFilePath);
// 	}
// 	else{
// 		std::cout << "Contenu du fichier:" << std::endl;
// 		std::cout << content << std::endl;
// 	}

// 	// test remove comments
// 	removeComments(content);
// 	std::cout << "Contenu après suppression des commentaires:" << std::endl;
// 	std::cout << content << std::endl;

// 	// test tokenize
// 	tokenize(content);
// 	// verif avec print pour debug 
// 	std::cout << "Tokens:" << std::endl;
// 	for (size_t i = 0; i < _tokens.size(); i++) {
// 		std::cout << "Token " << i << ": " << _tokens[i] << std::endl;
// 	}

// 	// Parse server
// 	// boucle pour trouver "server", et appelle parseServeur
// 	// init de l'iterateur 
// 	std::vector<std::string>::iterator it = _tokens.begin();
// 	// parcourir les tokens
// 	while (it != _tokens.end()){
// 		if (*it == "server"){
// 			parseServer(it);
// 		}
// 		else{
// 			throw ParseException("Est ce qu'il y a une erreur ici ou pas ?? rajouter + *it ou pas ? ");
// 		}
// 	}
// }










// void WebServer::initSockets() {
//     std::map<std::string, SocketServer> serversByPort;

//     for (std::vector<ServerConfig>::iterator it = _servers.begin(); it != _servers.end(); ++it) {
//         std::string port = it->port;

//         // Si le port n'existe pas encore → créer le socket
//         if (serversByPort.find(port) == serversByPort.end()) {
//             serversByPort.insert(std::make_pair(port, SocketServer(port)));
//         }

//         // Ajouter la config au socket correspondant
//         serversByPort[port].addConfig(&(*it));
//     }

//     // Copier dans ton vector final
//     for (std::map<std::string, SocketServer>::iterator it = serversByPort.begin();
//          it != serversByPort.end(); ++it) {
//         _socketServers.push_back(it->second);
//     }
// }
















// void testPrintSocket() const {
//     std::cout << "=== WebServer SocketServers ===" << std::endl;

//     int socketCount = 0;

//     for (std::vector<SocketServer>::const_iterator sockIt = _socketServers.begin();
//          sockIt != _socketServers.end(); ++sockIt) {
//         const SocketServer& sock = *sockIt;

//         std::cout << "\n[" << ++socketCount << "] SocketServer" << std::endl;
//         std::cout << "  Port: " << sock.getPort() << std::endl;
//         std::cout << "  Socket FD: " << sock.getFd() << std::endl;

//         const std::vector<const ServerConfig*>& servers = sock.getServers();
//         if (servers.empty()) {
//             std::cout << "  No ServerConfig attached!" << std::endl;
//             continue;
//         }

//         int serverCount = 0;
//         for (std::vector<const ServerConfig*>::const_iterator servIt = servers.begin();
//              servIt != servers.end(); ++servIt) {
//             const ServerConfig* server = *servIt;

//             std::cout << "  [" << ++serverCount << "] ServerConfig" << std::endl;
//             std::cout << "    MaxBodySize: " << server->maxBodySize << std::endl;

//             const std::vector<std::string>& index = server->index;
//             std::cout << "    Index files:";
//             if (index.empty()) std::cout << " (none)" << std::endl;
//             else {
//                 std::cout << std::endl;
//                 for (std::vector<std::string>::const_iterator idxIt = index.begin();
//                      idxIt != index.end(); ++idxIt) {
//                     std::cout << "      - " << *idxIt << std::endl;
//                 }
//             }

//             const std::map<int,std::string>& errors = server->errorPages;
//             std::cout << "    Error pages:";
//             if (errors.empty()) std::cout << " (none)" << std::endl;
//             else {
//                 std::cout << std::endl;
//                 for (std::map<int,std::string>::const_iterator errIt = errors.begin();
//                      errIt != errors.end(); ++errIt) {
//                     std::cout << "      " << errIt->first << " -> " << errIt->second << std::endl;
//                 }
//             }

//             const std::vector<LocationConfig>& locations = server->locations;
//             std::cout << "    Locations:";
//             if (locations.empty()) std::cout << " (none)" << std::endl;
//             else {
//                 std::cout << std::endl;
//                 int locCount = 0;
//                 for (std::vector<LocationConfig>::const_iterator locIt = locations.begin();
//                      locIt != locations.end(); ++locIt) {
//                     const LocationConfig& loc = *locIt;
//                     std::cout << "      [" << ++locCount << "] Location" << std::endl;
//                     std::cout << "        Path: " << loc.path << std::endl;
//                     std::cout << "        Root: " << loc.root << std::endl;

//                     const std::vector<std::string>& locIndex = loc.index;
//                     std::cout << "        Index files:";
//                     if (locIndex.empty()) std::cout << " (none)" << std::endl;
//                     else {
//                         std::cout << std::endl;
//                         for (std::vector<std::string>::const_iterator idxIt2 = locIndex.begin();
//                              idxIt2 != locIndex.end(); ++idxIt2) {
//                             std::cout << "          - " << *idxIt2 << std::endl;
//                         }
//                     }

//                     const std::vector<std::string>& methods = loc.methods;
//                     std::cout << "        Allowed Methods:";
//                     if (methods.empty()) std::cout << " (none)" << std::endl;
//                     else {
//                         std::cout << " ";
//                         for (std::vector<std::string>::const_iterator mIt = methods.begin();
//                              mIt != methods.end(); ++mIt) {
//                             std::cout << *mIt << " ";
//                         }
//                         std::cout << std::endl;
//                     }
//                 }
//             }

//         } // end ServerConfig loop
//     } // end SocketServer loop

//     std::cout << "\n=== End of WebServer Info ===" << std::endl;
// }





// void	WebServer::pollLoop(){
// 	initPollStruct();
// 	LOG("---- ANCIENNE POLL LOOP ----"); // --------------------

// 	while (_running){

// 		int ret = poll(&_pollFds[0], _pollFds.size(), 1000);
// 		LOG("poll() ret = " << ret);

// 		if (ret == -1) {
// 			if (errno == EINTR) {
// 				_running = false;
// 				break;
// 			} else 
// 				throw RunningException(std::string("Poll: ") + strerror(errno));
// 		}
		
// 		checkTimeouts();

// 		if (ret == 0)
// 			continue;

// 		size_t size = _pollFds.size();
// 		for (size_t i = 0; i < size; ++i){

// 			LOG("Checking fd: " << _pollFds[i].fd  << " revents: " << _pollFds[i].revents);
			
// 			if (_pollFds[i].revents & (POLLHUP | POLLERR)) {
// 				closeConnection(_pollFds[i].fd);
// 				//--i; //---------------------------------------------------------------------
// 				continue;
// 			}
			
// 			if (_pollFds[i].revents & POLLIN){
// 				int fd = _pollFds[i].fd;
// 				LOG("POLLIN on fd " << fd); 

// 				if (isServerFd(fd)){
// 					try { acceptClient(fd); }
// 					catch (const ResponseException& e) {
// 						sendResponse(e.getFd(), e.getCode());
// 					}
// 				}
// 				else {
// 					try { handleRequest(fd); }
// 					catch (const ResponseException& e) {
// 						sendResponse(e.getFd(), e.getCode());
// 					}
// 				}
// 			}
// 			if (_pollFds[i].revents & POLLOUT){
// 				int fd = _pollFds[i].fd;
// 				LOG("POLLOUT on fd " << fd); // --------------------
// 				try { sendResponse(fd, 0); }
// 				catch (...) { closeConnection(fd); }
// 			}
// 		}
// 	}
// }





//lui trouver un dossier adéquat
// void	WebServer::checkTimeouts() {
//     time_t now = std::time(NULL);

//     for (std::map<int, SocketClient*>::iterator it = _socketClients.begin();
//          it != _socketClients.end(); ) {

//         SocketClient* client = it->second;
//         int fd = it->first;

//         if (now - client->lastActivity > 10) {
//             LOG("Timeout client fd = " << fd); // --------------------
//             closeConnection(fd);
//             it = _socketClients.erase(it);
//         } else {
//             ++it;
//         }
//     }
// }

// OLD VERSION ( a delete plus tard)
// std::string generateId() {
// 	std::stringstream ss;
// 	ss << getpid() << "_" << std::time(NULL);

// 	// std::stringstream ss;
// 	// ss << "/tmp/webserv_body_" << getpid() << "_" << time(NULL) << "_" << rand();

// 	// static size_t counter = 0;
// 	// std::stringstream ss;
// 	// ss << "/tmp/webserv_body_" << getpid() << "_" << counter++;

// 	return ss.str();
// }

// void	HttpRequest::openBodyFile() {
// 	if (_bodyFile.is_open())
//         return;

// 	_bodyFilePath = "/tmp/webserv_body_" + generateId();
// 	LOG(" --------------------------- le body path au moment de sa divine creation : " << _bodyFilePath);
// 	_bodyFile.open(_bodyFilePath.c_str(), std::ios::binary);
// 	if (!_bodyFile.is_open()) {
// 		throw std::runtime_error("Failed to open body file"); // en attendant est ce que il faut envoyer un error 500
// 	}
// }





// HttpResponse WebServer::buildErrorResponse(int code, const SocketClient* client) {
//     HttpResponse res;

//     std::string statusText = "Error";
// 	std::string	imagePath = "Error";
// 	std::string comment = "";

// 	if (code == 400){
// 		statusText = "Bad Request";
// 		//imagePath = "./doss";
// 		comment = "bibibibb 400";
// 	}
// 	else if (code == 403){
// 		statusText = "Forbidden";
// 		//imagePath = "./doss";
// 		comment = "FOR BI DEN bibibibibbi";
// 	}
//     else if (code == 404){
//         statusText = "Not Found";
// 		imagePath = "./Assets/404.jpg";
// 		comment = "Oups... Prout Dino ate your page 🦖";
// 	}
// 	else if (code == 413){
//         statusText = "Payload too large";
// 		//imagePath = "./doss";
// 		comment = "413 bibibbibibi ";
// 	}
// 	else if (code == 414){
//         statusText = "URI too long";
// 		//imagePath = "./doss";
// 		comment = "414 bibibibiibibibbi ";
// 	}
// 	else if (code == 415){
//         statusText = "Unsupported media type";
// 		//imagePath = "./doss";
// 		comment = "415 bibibibiibibibbi ";
// 	}
// 	else if (code == 418){
//         statusText = "I am a teapot";
// 		//imagePath = "./doss";
// 		comment = "The Server refuses to brew Coffee because it is permanently a TeaPot";
// 	}
// 		else if (code == 431){
//         statusText = "Request Header fields too large";
// 		//imagePath = "./doss";
// 		comment = "431 bibibibiibibibbi ";
// 	}
//     else if (code == 500){
// 		statusText = "Internal Server Error";
// 		//imagePath = "./doss";
// 	}
    
// 	// Status line
//     res.statusLine = "HTTP/1.1 " + longToString(code) + " " + statusText;

//     // Body
//     res.body =
//         "<html>"
//         "<head><title>" + longToString(code) + " " + statusText + "</title></head>"
//         "<body style='text-align:center;'>"
//         "<h1>" + longToString(code) + " - " + statusText + "</h1>"
//         "<p>" + comment + "</p>"
//         //"<img src='" + imagePath + "' width='400'>"
//         "</body>"
//         "</html>";

//     // Headers
// 	std::string sizeStr = longToString(res.body.size());
//     res.headers["Content-Length"].push_back(sizeStr);
//     res.headers["Content-Type"].push_back("text/html");
// 	if (client->keepAlive)
//         res.headers["Connection"].push_back("keep-alive");
// 	else 
//     	res.headers["Connection"].push_back("close");

//     return res;
// }

// void WebServer::checkTimeouts() {
//     for (size_t i = 0; i < _pollFds.size(); ) {

//         int fd = _pollFds[i].fd;

//         if (!isServerFd(fd)) {
//             std::map<int, SocketClient*>::iterator it = _socketClients.find(fd);
// 			if (it == _socketClients.end()) {
// 				i++;
// 				continue;
// 			}

// 		SocketClient* client = it->second;

//             if (client && isTimedOut(client)) {
//                 LOG("Timeout client fd = " << fd);
//                 closeConnection(fd);
//                 continue;
//             }
//         }

//         i++;
//     }
// }

// Note pour resolve query

	//test?key1=val1&key2=val2 -> key1 = "val1"     key2 = "val2"
	//test?key1=&key2=val2 -> key1 = ""     key2 = "val2"
	//test?key1 -> key1 = ""
	//test?key1=val1=key2=val2 -> key1 = "val1=key2=val2"
	
	//si 2x les meme on garde le 2e
	// %20 -> espace
	// + -> espace 
	// faire gestion generale des % par contre si pas hexa -> badrequest -> hexToChar(dejac codee) 
	// si \0 bad request