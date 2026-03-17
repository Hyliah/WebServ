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
	path = "/";
	root = ""; // si location.root est vide, utilise server.root par defaut ?? a checker 
	autoindex = false;
	index.push_back("index.html");
	returnUrl = "";
	uploadStore = "";
	maxBodySize = 100000; // 100ko par defaut
	cgiEnabled = false;

	methods.push_back("GET"); // par defaut on autorise que GET

	hasGet = true;
	hasPost = false;
	hasDelete = false;
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
	maxBodySize = other.maxBodySize;
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
		maxBodySize = other.maxBodySize;
		cgiEnabled = other.cgiEnabled;
	}
	return *this;
}
LocationConfig::~LocationConfig() {}

