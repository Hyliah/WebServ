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
