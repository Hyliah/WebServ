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
#include "utilsGeneral.hpp"
#include "WebServer.hpp"
#include "Exceptions.hpp"
#include <iostream>
#include <sstream>
#include <fstream>


/* *************************************************** */
/*  Constructors, destructor, and assignment operator  */
/* *************************************************** */ 
ParserConfig::ParserConfig() {}
ParserConfig::ParserConfig(const ParserConfig &other) {
	_servers = other._servers;
	_tokens = other._tokens;
}
ParserConfig &ParserConfig::operator=(const ParserConfig &other) {
	if (this != &other) {
		_servers = other._servers;
		_tokens = other._tokens;
	}
	return *this;
}
ParserConfig::~ParserConfig() {}

/* *************************************************** */
/*  Fonction parse principale                          */
/* *************************************************** */ 
// VERSION CLEAN :
void ParserConfig::parse(const std::string &configFilePath){
	std::string content = readFile(configFilePath);
	removeComments(content);
	checkBracketsBalance(content);
	tokenize(content);
	std::vector<std::string>::iterator it = _tokens.begin();
	while (it != _tokens.end()){
		if (*it == "server"){
			parseServer(it);
		}
		else{
			throw ParseException(CONF, "Unexpected token: " + *it);
		}
	}
}

/* *************************************************** */
/*  Read, clean file + tokenize                        */
/* *************************************************** */ 
// lire le fichier et retourner son contenu sous forme de string
// fichier et non dossier ??? check si ok 
std::string ParserConfig::readFile(const std::string &path) {
	std::ifstream file(path.c_str());
	if (!file.is_open()) {
		throw ParseException(CONF, "Could not open config file: " + path);
	}
	std::stringstream buffer;
	buffer << file.rdbuf();
	file.close();
	if (buffer.str().empty()) {
		throw ParseException(CONF, "Config file is empty: " + path);
	}
	return buffer.str();
}

void ParserConfig::removeComments(std::string &content) {
	size_t pos = content.find('#');
	while (pos != std::string::npos) {
		size_t endOfLine = content.find('\n', pos);
		if (endOfLine == std::string::npos) {
			content.erase(pos);
			break;
		} else {
			content.erase(pos, endOfLine - pos);
		}
		pos = content.find('#', pos);
	}
}

// TOKENIZEEEERRR
void ParserConfig::tokenize(const std::string &content) {
	std::string result;
	for (size_t i = 0; i < content.size(); i++) {
		if (content[i] == '{' || content[i] == '}' || content[i] == ';') {
			result += ' ';
			result += content[i];
			result += ' ';
		}
		else {
			result += content[i];
		}
	}
	std::stringstream buffer(result);
	std::string singletoken;
	while (buffer >> singletoken) {
		_tokens.push_back(singletoken);
	}
}

/* *************************************************** */
/*  PARSE SERVEUR ET LOCATION                          */
/* *************************************************** */ 
// a changer pour eviter foret de if ? pointeur sur fonction ? check deja si tt marche ... 
void	ParserConfig::parseServer(std::vector<std::string>::iterator &it){
	it++;
	if ( it == _tokens.end() || *it != "{")
		throw ParseException(CONF, "expected '{' after 'server'");
	it++;

	ServerConfig newServer;

	while (it != _tokens.end() && *it != "}"){
		if (*it == "listen")
			handleListen(it, newServer);
		else if (*it == "host")
			handleHost(it, newServer);
		else if (*it == "server_name")
			handleServerName(it, newServer);
		else if (*it == "error_page")
			handleErrorPage(it, newServer);
		else if (*it == "client_max_body_size")
			handleMaxBodySize(it, newServer);
		else if (*it == "root")
			handleRoot(it, newServer);
		else if (*it == "index")
			handleIndex(it, newServer);
		else if (*it == "location")
			parseLocation(it, newServer);
		else
			throw ParseException(CONF, "Unknown directive: " + *it);
	}
	if (it == _tokens.end())
		throw ParseException(CONF, "Missing '}' at the end of the block");
	it++;
	_servers.push_back(newServer);
}

void	ParserConfig::parseLocation(std::vector<std::string>::iterator &it, ServerConfig &server){
	it++;
	if ( it == _tokens.end())
		throw ParseException(CONF, "Location needs a path");

	LocationConfig newLocation;
	newLocation.path = *it;
	it++;

	if (it == _tokens.end() || *it != "{")
		throw ParseException(CONF, "need '{' after location path ");
	it++;

	while (it != _tokens.end() && *it != "}"){
		if (*it == "root")
			handleRoot(it, newLocation);
		else if (*it == "allow_methods" || *it == "methods")
			handleMethods(it, newLocation);
		else if (*it == "autoindex")
			handleAutoindex(it, newLocation);
		else if (*it == "index")
			handleIndex(it, newLocation);
		else if (*it == "client_max_body_size")
			handleMaxBodySize(it, newLocation);
		else if (*it == "return")
			handleReturn(it, newLocation);
		else if (*it == "upload_store")
			handleUploadStore(it, newLocation);
		else if (*it == "cgi_info")
			handleCgi(it, newLocation);
		else if (*it == "cgi_enabled")
    		handleCgiEnabled(it, newLocation);
		else	
			throw ParseException(CONF, "Unknown directive: " + *it);
	}
	if (it == _tokens.end())
		throw ParseException(CONF, "Missing '}' at the end of the block");
	it++;
	server.locations.push_back(newLocation);
}

/* *************************************************** */
/*  HANDLERS SERVER                                    */
/* *************************************************** */ 

void ParserConfig::handleListen(std::vector<std::string>::iterator &it, ServerConfig &server)
{
    it++;
    if (!validateValue(it))
        throw ParseException(CONF, "'listen' directive needs a value (e.g., 8080)");

    server.port = *it;

    // verif que le port est bien un nombre
    for (size_t i = 0; i < server.port.size(); i++)
    {
        if (!isdigit(server.port[i]))
            throw ParseException(CONF, "invalid port value: " + server.port);
    }
    // verif que le port est dans la plage valide
    int port = stringToInt(server.port);
    if (port <= 0 || port > 65535)
        throw ParseException(CONF, "port out of range (1-65535): " + server.port);

    it++;
    checkSemicolon(it);
}
void ParserConfig::handleHost(std::vector<std::string>::iterator &it, ServerConfig &server)
{
    it++;
    if (!validateValue(it))
        throw ParseException(CONF, "'host' directive needs a value (e.g., 127.0.0.1)");
    server.host = *it;
    it++;
    checkSemicolon(it);
}

void	ParserConfig::handleServerName(std::vector<std::string>::iterator &it, ServerConfig &server){
	it++;
	if (!validateValue(it))
		throw ParseException(CONF, "server_name needs a value");
	server.serverName = *it;
	it++;
	checkSemicolon(it);
}

void	ParserConfig::handleErrorPage(std::vector<std::string>::iterator &it, ServerConfig &server){
	it++;
	std::vector<int> codes;
	// On récupère tous les nombres (les codes d'erreur)
	while (it != _tokens.end() && isdigit((*it)[0])) {
		codes.push_back(stringToInt(*it));
		it++;
	}
	// verif qu'on a au moins un code et qu'il reste un token (le chemin)
	if (codes.empty())
		throw ParseException(CONF, "error_page needs at least one error code");
	if (!validateValue(it))
		throw ParseException(CONF, "error_page directive needs a file path after the codes");
	std::string errorPath = *it; // Le token actuel est le chemin (ex: /404.html)
	it++;
	// remplit la map pour chaque code trouvé
	for (size_t i = 0; i < codes.size(); i++) {
		server.errorPages[codes[i]] = errorPath;
	}
	checkSemicolon(it);
}

void	ParserConfig::handleMaxBodySize(std::vector<std::string>::iterator &it, ServerConfig &server){
	it++;
	if (!validateValue(it))
		throw ParseException(CONF, "client_max_body_size needs a value");
	server.maxBodySize = parseSize(*it);
	it++;
	checkSemicolon(it);
}

void	ParserConfig::handleRoot(std::vector<std::string>::iterator &it, ServerConfig &server){
	it++;
	if (!validateValue(it))
		throw ParseException(CONF, "Root needs a value");
	server.root = *it;
	it++;
	checkSemicolon(it);
}

void	ParserConfig::handleIndex(std::vector<std::string>::iterator &it, ServerConfig &server){
	it++;
	server.index.clear();
	if (!validateValue(it))
		throw ParseException(CONF, "Index needs at least one value");
	while ( it != _tokens.end() && *it != ";"){
		server.index.push_back(*it);
		it++;
	}
	checkSemicolon(it);
}

/* *************************************************** */
/*  HANDLERS LOCATION                                  */
/* *************************************************** */ 
void	ParserConfig::handleRoot(std::vector<std::string>::iterator &it, LocationConfig &location){
	it++;
	if (!validateValue(it))
		throw ParseException(CONF, "Root needs a value");
	location.root = *it;
	it++;
	checkSemicolon(it);
}

void	ParserConfig::handleMethods(std::vector<std::string>::iterator &it, LocationConfig &location){
	it++;
	location.methods.clear();
	location.hasGet = false;
	location.hasPost = false;
	location.hasDelete = false;
	if (!validateValue(it))
		throw ParseException(CONF, "Need at least one methode : GET, POST, DELETE");
	while (it != _tokens.end() && *it != ";"){
		if (*it == "GET")
			location.hasGet = true;
		else if (*it == "POST")
			location.hasPost = true;
		else if (*it == "DELETE")
			location.hasDelete = true;
		else
			throw ParseException(CONF, "Invalid HTTP method: " + *it);
		location.methods.push_back(*it);
		it++;
	}
	checkSemicolon(it);
}

void	ParserConfig::handleAutoindex(std::vector<std::string>::iterator &it, LocationConfig &location){
	it++;
	if (!validateValue(it))
		throw ParseException(CONF, "Autoindex needs a value (on/off)");
	if (*it == "on")
		location.autoindex = true;
	else if (*it == "off")
		location.autoindex = false;
	else
		throw ParseException(CONF, "Invalid value for autoindex: " + *it);
	it++;
	checkSemicolon(it);
}

void	ParserConfig::handleIndex(std::vector<std::string>::iterator &it, LocationConfig &location){
	it++;
	location.index.clear();
	if (!validateValue(it))
		throw ParseException(CONF, "Index needs at least one value");
	while ( it != _tokens.end() && *it != ";"){
		location.index.push_back(*it);
		it++;
	}
	checkSemicolon(it);
}

void	ParserConfig::handleMaxBodySize(std::vector<std::string>::iterator &it, LocationConfig &location){
	it++;
	if (!validateValue(it))
		throw ParseException(CONF, "client_max_body_size needs a value");
	location.maxBodySize = parseSize(*it);
	it++;
	checkSemicolon(it);
}

void	ParserConfig::handleReturn(std::vector<std::string>::iterator &it, LocationConfig &location){
    it++;
    if (!validateValue(it))
        throw ParseException(CONF, "return needs a code and/or URL");

    if (isdigit((*it)[0])) {
        location.returnCode = stringToInt(*it);
        it++;
    }
    if (validateValue(it) && *it != ";") {
        location.returnUrl = *it;
        it++;
    }
    checkSemicolon(it);
}

void	ParserConfig::handleUploadStore(std::vector<std::string>::iterator &it, LocationConfig &location){
	it++;
	if (!validateValue(it))
		throw ParseException(CONF, "upload_store needs a value");
	location.uploadStore = *it;
	it++;
	checkSemicolon(it);
}

void ParserConfig::handleCgi(std::vector<std::string>::iterator &it, LocationConfig &location){
    it++;

    if (!validateValue(it))
        throw ParseException(CONF, "CGI directive needs an extension (e.g., .py)");
    std::string ext = *it;
	LOG("----------------extension : " << ext);
    it++;

    if (!validateValue(it))
        throw ParseException(CONF, "CGI directive needs a path");
    std::string path = *it;
	LOG("----------------path : " << path);
    it++;

    location.cgiInfo[ext] = path;
	std::cout << location.cgiInfo[ext] << std::endl;
    location.cgiEnabled = true; // OK ici aussi, mais redondant si tu utilises empty()
    checkSemicolon(it);
}

void ParserConfig::handleCgiEnabled(std::vector<std::string>::iterator &it, LocationConfig &location)
{
    it++;
    if (!validateValue(it))
        throw ParseException(CONF, "cgi_enabled needs on/off");

    if (*it == "on")
        location.cgiEnabled = true;
    else if (*it == "off")
        location.cgiEnabled = false;
    else
        throw ParseException(CONF, "cgi_enabled must be on/off");

    it++;
    checkSemicolon(it);
}

// void	ParserConfig::handleCgi(std::vector<std::string>::iterator &it, LocationConfig &location){
// 	it++; 
// 	// recup l'extension (ex: .py)
// 	if (!validateValue(it))
// 		throw ParseException(CONF, "CGI directive needs an extension (e.g., .py)");
// 	std::string ext = *it;
// 	it++;
// 	// récupère le chemin ex: /usr/bin/python3)
// 	if (!validateValue(it))
// 		throw ParseException(CONF, "CGI directive needs a path ");
// 	std::string path = *it;
// 	it++;
// 	location.cgiInfo[ext] = path;
// 	location.cgiEnabled = !location.cgiInfo.empty();
// 	checkSemicolon(it);
// }


/* *************************************************** */
/*  CHECKS AND UTILS                                   */
/* *************************************************** */ 

// finalement on fait une petite verif structurelle pour pas que la classe soit remplie avec de la daube
// mais on check les trucs plus speficique ds la fonction finale de verif config 
bool	ParserConfig::validateValue(std::vector<std::string>::iterator &it){
	if (it == _tokens.end() || *it == ";" || *it == "{" || *it == "}") {
		throw ParseException(CONF, "Unexpected token or missing value");
	}
	return true;
}

void	ParserConfig::checkSemicolon(std::vector<std::string>::iterator &it){
	if (it == _tokens.end() || *it != ";")
		throw ParseException(CONF, "Need ';' at the end of directives");
	it++;
}

// ca aussi, petite verif structurelle aussi, meme si ds les fonctions on verifie les brracket pour voir leur contenu 
void	ParserConfig::checkBracketsBalance(const std::string &content){
	int balance = 0;
	for (size_t i = 0; i < content.size(); ++i) {
		if (content[i] == '{')
			balance++;
		else if (content[i] == '}')
			balance--;
		if (balance < 0) {
			throw ParseException(CONF, "Unexpected '}' found");
		}
	}
	if (balance != 0) {
		throw ParseException(CONF, "Brackets are not balanced");
	}
}


/* *************************************************** */
/*  FINAL VERIF'                                       */
/* *************************************************** */ 

void	ParserConfig::verifyConfig(){
	// verifie que chaque serveur a au moins un port et une location, et qu'il n'y a pas de doublons de ports
	// etc ... 
	if (_servers.empty())
        throw ParseException(CONF, "No server block found");

    // Check doublons host:port
    for (size_t i = 0; i < _servers.size(); i++) {
        for (size_t j = i + 1; j < _servers.size(); j++) {
            if (_servers[i].host == _servers[j].host && 
                _servers[i].port == _servers[j].port)
                throw ParseException(CONF, "Duplicate host:port: " 
                    + _servers[i].host + ":" + _servers[i].port);
        }
    }
    for (size_t i = 0; i < _servers.size(); i++) {
        ServerConfig &s = _servers[i];

        // Port obligatoire (même si tu as une valeur par défaut)
        if (s.port.empty())
            throw ParseException(CONF, "Server missing 'listen' directive");

        // Root obligatoire
        if (s.root.empty())
            throw ParseException(CONF, "Server missing 'root' directive");

        // Vérif locations
        for (size_t j = 0; j < s.locations.size(); j++) {
            LocationConfig &loc = s.locations[j];

            if (loc.path.empty() || loc.path[0] != '/')
                throw ParseException(CONF, "Location path must start with '/'");

            // Si CGI activé, vérifier que les extensions ont bien un exécutable
            if (loc.cgiEnabled && loc.cgiInfo.empty())
                throw ParseException(CONF, "CGI enabled but no cgi_info defined");
        }
    }
}


// ------------------------------ Prise de note vrac ------------------------------
// parser les blocs de config (server, location) 
// et stocker les infos dans des classes serverConfig et locationConfig
// donc si on croise server { on cree objet serverConfig et on parse jusqu'a la fin du bloc, pareil pour location
// tant qu'on croise pas } on continue de parser et stocker les infos dans la struc correspondante

// si oncroise location /path { on cree objet locationConfig et on parse jusqu'a la fin du bloc
// tant qu'on croise pas } on continue de parser et stocker les infos dans la struc locationConfig

// pour stocker les donnees on va utiliser conteneur vector ? 
// chaque serverconfig va contenir les ports, nom de domaine, et le vecteur locationconfig
// chaque locationconfig va contenir le path, methodes autorisees, les directives de location (root, index, etc), autoindex etc...

// erreur de config exemple :
// accolades bien fermees
// validite des nombres pour les ports (0-65535)
// validite des methodes (GET, POST, DELETE)
// directives valides (root, index, autoindex, etc...)
// quitter de maniere clean avec mess d'erreur clair si fichier pas bien formaté ou directives invaldies
// -> ne pas lancer le serveur si la config est pourrie, obviously

// PAR DEFAUT ??
// Si non spécifié dans le config :
// - `port` : 80
// - `host` : 0.0.0.0
// - `client_max_body_size` : 1M
// - `index` : index.html
// - `autoindex` : off
// - `methods` : GET

// POUR LOCATION ???
// - Remplit les infos spécifiques à la route.
// - Ajoute la `LocationConfig` au serveur en cours.

// attention pour les locations, garder le match le plus long 
// par ex :
/*
	/
	/images
	/images/icons
*/
// On va garder la 3eme option
// donc parser en gardant ca en tete 