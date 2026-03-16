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

/* *************************************************** */
/*  Constructors, destructor, and assignment operator  */
/* *************************************************** */ 
ServerConfig::ServerConfig(){
	// voir pour le truc par defaut
	// tout mettre dans le constructeur, recheck les valeurs par defaut 
	port = "8080"; // standard pour test 
	host = "0.0.0.0"; // ecoute sur toutes les interfaces 
	serverName = "default_server";
	root = "./www";
	maxBodySize = 1000000;

	// les conteneur sont init vide pas defaut donc ok

}
ServerConfig::ServerConfig(const ServerConfig &other) {
	port = other.port;
	host = other.host;
	serverName = other.serverName;
	root = other.root;
	maxBodySize = other.maxBodySize;
	errorPages = other.errorPages;
	locations = other.locations;
}
ServerConfig& ServerConfig::operator=(const ServerConfig &other) {
	if (this != &other) {
		port = other.port;
		host = other.host;
		serverName = other.serverName;
		root = other.root;
		maxBodySize = other.maxBodySize;
		errorPages = other.errorPages;
		locations = other.locations;
	}
	return *this;
}
ServerConfig::~ServerConfig() {}

