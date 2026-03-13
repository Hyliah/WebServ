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

#ifndef SERVERCONFIG_HPP
#define SERVERCONFIG_HPP

#include "Webserv.hpp"
#include "LocationConfig.hpp"

class ServerConfig {
public:
	std::string					port; 		// str pour getadrinfo() pour eviter hton 
	std::string					host;         // par ex par defaut "0.0.0.0"
	std::string					serverName;

	size_t						maxBodySize;
	
	std::map<int, std::string>  errorPages;  // Code (404) -> Chemin du fichier
	std::vector<LocationConfig> locations;

	ServerConfig(); //: port("8080"), maxBodySize(1000000) {} // voir pour le truc par defaut
};

#endif

// info en pointeur ou pas ? pour hitoire de socket 
// pas de pointeurs ici finalement