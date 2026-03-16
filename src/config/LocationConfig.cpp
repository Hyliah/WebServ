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

#include "LocationConfig.hpp"

/* *************************************************** */
/*  Constructors, destructor, and assignment operator  */
/* *************************************************** */ 
LocationConfig::LocationConfig(){
	// voir si on met des trucs par defaut ou pas
	// chack ca later c'est juste pour faire des tests
	// oui tout mettre dans le constructeur, recheck les valeurs par defaut 
	path = "/";
	root = ""; // si location.root est vide, utilise server.root par defaut ?? a checker 
	autoindex = false;
	index = "index.html";
	returnUrl = "";
	uploadStore = "";
	cgiEnabled = false;

	methods.push_back("GET"); // par defaut on autorise que GET
}
LocationConfig::LocationConfig(const LocationConfig &other) {
	path = other.path;
	root = other.root;
	methods = other.methods;
	autoindex = other.autoindex;
	index = other.index;
	returnUrl = other.returnUrl;
	uploadStore = other.uploadStore;
	cgiInfo = other.cgiInfo;
	cgiEnabled = other.cgiEnabled;
}
LocationConfig& LocationConfig::operator=(const LocationConfig &other) {
	if (this != &other) {
		path = other.path;
		root = other.root;
		methods = other.methods;
		autoindex = other.autoindex;
		index = other.index;
		returnUrl = other.returnUrl;
		uploadStore = other.uploadStore;
		cgiInfo = other.cgiInfo;
		cgiEnabled = other.cgiEnabled;
	}
	return *this;
}
LocationConfig::~LocationConfig() {}

