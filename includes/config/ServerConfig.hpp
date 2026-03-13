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
    ServerConfig(const ServerConfig& other);
    ServerConfig& operator=(const ServerConfig& other);
    ~ServerConfig();
};

#endif

// info en pointeur ou pas ? pour hitoire de socket 
// pas de pointeurs ici finalement


//mettre dans le .cpp
void initServerConfig(ServerConfig &obj){
    obj.port = "default";
    obj.host = "default";
    obj.serverName = "default";
    obj.maxBodySize = 100; //vraiment a voir si on met -1 si on estime que si y a pas y a pas (pas de palais, ... pas de palais)

    //mettre dans la verif un if (location.size() < 1) -> FALSE

}