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

#include "ServerConfig.hpp"
#include "ParserConfig.hpp"

ServerConfig::ServerConfig() : port("8080"), maxBodySize(1000000) {
	// voir pour le truc par defaut
	// tout mettre dans le constructeur, recheck les valeurs par defaut 
}
ServerConfig::ServerConfig(const ServerConfig &other) {
	port = other.port;
	host = other.host;
	serverName = other.serverName;
	maxBodySize = other.maxBodySize;
	errorPages = other.errorPages;
	locations = other.locations;
}
ServerConfig& ServerConfig::operator=(const ServerConfig &other) {
	if (this != &other) {
		port = other.port;
		host = other.host;
		serverName = other.serverName;
		maxBodySize = other.maxBodySize;
		errorPages = other.errorPages;
		locations = other.locations;
	}
	return *this;
}
ServerConfig::~ServerConfig() {}

