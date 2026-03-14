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

// below pour test, a verif et modif 

// CANONICAL FORM
LocationConfig::LocationConfig() : autoindex(false), cgiEnabled(false) {
	// voir si on met des trucs par defaut ou pas
	// chack ca later c'est juste pour faire des tests
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
LocationConfig::~LocationConfig() {
}

