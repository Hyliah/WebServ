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

// CANONICAL FORM

ParserConfig::ParserConfig() {
}	

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

ParserConfig::~ParserConfig() {
}

// FONCTION PARSE PRINCIPALE
// debut pour test readfile only + comment
// a continuer 
// et enlever les cout pour le test final
void ParserConfig::parse(const std::string &configFilePath) {
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
}

// CLEAN ET TOKEN

// lire le fichier et retourner son contenu sous forme de string
// doit verif ouverture
// fichier et non dossier ??? check si ok 
// et pas vide 
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



// token:
// couper le fichier en token pour chaque mot


// recursion pour parser les blocs de config (server, location) 
// et stocker les infos dans des struc serverConfig et locationConfig
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

