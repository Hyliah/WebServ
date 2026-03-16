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
void ParserConfig::parse(const std::string &configFilePath) {

	// VERSION SANS TEST
	// std::string content = readFile(configFilePath);
	// ... ( if empty ) throw error ... 
	// removeComments(content);
	// tokenize(content);


	// test readfile
	std::string content = readFile(configFilePath);
	if (content.empty()) {
		throw ParseException("Error: Config file is empty: " + configFilePath);
	}
	else{
		std::cout << "Contenu du fichier:" << std::endl;
		std::cout << content << std::endl;
	}

	// test remove comments
	removeComments(content);
	std::cout << "Contenu après suppression des commentaires:" << std::endl;
	std::cout << content << std::endl;

	// test tokenize
	tokenize(content);
	// verif avec print pour debug 
	std::cout << "Tokens:" << std::endl;
	for (size_t i = 0; i < _tokens.size(); i++) {
		std::cout << "Token " << i << ": " << _tokens[i] << std::endl;
	}

	// Parse server
	// boucle pour trouver "server", et appelle parseServeur
	// init de l'iterateur 
	std::vector<std::string>::iterator it = _tokens.begin();
	// parcourir les tokens
	while (it != _tokens.end()){
		if (*it == "server"){
			parseServer(it);
		}
		else{
			throw ParseException("Est ce qu'il y a une erreur ici ou pas ?? rajouter + *it ou pas ? ");
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
		throw ParseException("Error: Could not open config file: " + path);
	}
	std::stringstream buffer;
	buffer << file.rdbuf();
	file.close();
	if (buffer.str().empty()) {
		throw ParseException("Error: Config file is empty: " + path);
	}
	return buffer.str();
}

 // pour retirer les commentaires 
void ParserConfig::removeComments(std::string &content) {
	size_t pos = content.find('#');
	while (pos != std::string::npos) {
		size_t endOfLine = content.find('\n', pos); // en partant de la pos du #
		if (endOfLine == std::string::npos) { // si c'est kla toute derniere ligen du fichier 
			content.erase(pos);
			break; // Plus rien à chercher après la fin du fichier
		} else {
			// On efface du '#' jusqu'au '\n' (sans supprimer le '\n')
			content.erase(pos, endOfLine - pos);
		}
		// On cherche le prochain '#' à partir de la position actuelle
		// pour ne pas reparcourir tout le début de la string
		pos = content.find('#', pos);
	}
}

// TOKENIZEEEERRR
void ParserConfig::tokenize(const std::string &content) {
	// gestion des spé
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
	// decoup des tokens 
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
	// - Boucle : tant que je ne vois pas `}`, je lis les clés (`listen`, `root`, etc.).
	// - Si je vois "location", j'appelle `parseLocation()`.

	it++; // pour skip le mot serveur
	if ( it == _tokens.end() || *it != "{")
		throw ParseException("Error : expected '{' after 'server'");
	it++; // skip accoldae

	ServerConfig newServer;

	while (it != _tokens.end() && *it != "}"){
		if (*it == "listen")
			handleListen(it, newServer);
		else if (*it == "server_name")
			handleServerName(it, newServer);
		else if (*it == "error_page")
			handleErrorPage(it, newServer);
		else if (*it == "client_max_body_size")
			handleMaxBodySize(it, newServer);
		else if (*it == "root")
			handleRoot(it, newServer);
		else if (*it == "location")
			parseLocation(it, newServer);
		else
			throw ParseException("Error : Unknown directive: " + *it);
	}
	if (it == _tokens.end())
		throw ParseException("Error : Missing '}' at the end of the block");
	it++; // skip accoldate fermenante
	_servers.push_back(newServer);
}

void	ParserConfig::parseLocation(std::vector<std::string>::iterator &it, ServerConfig &server){
	it++;
	if ( it == _tokens.end())
		throw ParseException("Location needs a path");

	LocationConfig newLocation;
	newLocation.path = *it;
	it++;

	if (it == _tokens.end() || *it != "{")
		throw ParseException("Error : need '{' after location path ");
	it++;

	while (it != _tokens.end() && *it != "}"){
		if (*it == "root")
			handleRoot(it, newLocation);
		else if (*it == "allow_methods")
			handleMethods(it, newLocation);
		else if (*it == "autoindex")
			handleAutoindex(it, newLocation);
		else if (*it == "index")
			handleIndex(it, newLocation);
		else if (*it == "return")
			handleReturn(it, newLocation);
		else if (*it == "upload_store")
			handleUploadStore(it, newLocation);
		else if (*it == "cgi_info")
			handleCgi(it, newLocation);
		else	
			throw ParseException("Error : Unknown directive: " + *it);
	}
	if (it == _tokens.end())
		throw ParseException("Error : Missing '}' at the end of the block");
	it++;
	server.locations.push_back(newLocation);
}

/* *************************************************** */
/*  HANDLERS SERVER                                    */
/* *************************************************** */ 
void	ParserConfig::handleListen(std::vector<std::string>::iterator &it, ServerConfig &server){
	// 1. Convertir *it en int (le port)
	// 2. it++;
	// 3. Vérifier si *it == ";"
	// if (*it != ";") throw std::runtime_error("';' manquant après le port");
	// utiliser la fontiond e fin de directive ?? 
}

void	ParserConfig::handleServerName(std::vector<std::string>::iterator &it, ServerConfig &server){

}

void	ParserConfig::handleErrorPage(std::vector<std::string>::iterator &it, ServerConfig &server){

}

void	ParserConfig::handleMaxBodySize(std::vector<std::string>::iterator &it, ServerConfig &server){

}

void	ParserConfig::handleRoot(std::vector<std::string>::iterator &it, ServerConfig &server){

}


/* *************************************************** */
/*  HANDLERS LOCATION                                  */
/* *************************************************** */ 
void	ParserConfig::handleRoot(std::vector<std::string>::iterator &it, LocationConfig &location){
	it++;
	if ( it == _tokens.end())
		throw ParseException("Error : Root needs a value");
	location.root = *it;
	it++;
	checkSemicolon(it);
}

void	ParserConfig::handleMethods(std::vector<std::string>::iterator &it, LocationConfig &location){

}

void	ParserConfig::handleAutoindex(std::vector<std::string>::iterator &it, LocationConfig &location){

}

void	ParserConfig::handleIndex(std::vector<std::string>::iterator &it, LocationConfig &location){

}

void	ParserConfig::handleReturn(std::vector<std::string>::iterator &it, LocationConfig &location){

}

void	ParserConfig::handleUploadStore(std::vector<std::string>::iterator &it, LocationConfig &location){

}

void	ParserConfig::handleCgi(std::vector<std::string>::iterator &it, LocationConfig &location){

}


/* *************************************************** */
/*  CHECKS AND UTILS                                   */
/* *************************************************** */ 
void	ParserConfig::checkSemicolon(std::vector<std::string>::iterator &it){
	if (it == _tokens.end() || *it != ";")
		throw ParseException("Error : Need ';' at the end of directives");
	it++;
}

void	ParserConfig::checkBracketsBalance(const std::string &content){

}

int		ParserConfig::stringToInt(const std::string &str){

}

size_t	ParserConfig::parseSize(const std::string &str){

}

/* *************************************************** */
/*  FINAL VERIF'                                       */
/* *************************************************** */ 
void	ParserConfig::verifyConfig(){

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