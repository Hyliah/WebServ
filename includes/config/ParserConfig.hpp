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

#ifndef PARSERCONFIG_HPP
#define PARSERCONFIG_HPP

#include "Webserv.hpp"

class ServerConfig; 
class LocationConfig;

class ParserConfig {

	private:
		std::vector<ServerConfig> 	_servers;
		std::vector<std::string> 	_tokens;

		// toutes les fonctions de parsing

		// CLEAN ET TOKEN
		std::string 				readFile(const std::string &path);
		void 						removeComments(std::string &content);
		std::vector<std::string> 	tokenize(const std::string &content);

		// PARSE SERVEUR ET LOCATION 
		void 						parseServer(std::vector<std::string>::iterator &it);
		void 						parseLocation(std::vector<std::string>::iterator &it, ServerConfig &server);

		// HANDLERS serveur
		void						handleListen(std::vector<std::string>::iterator &it, ServerConfig &server);
		void						handleServerName(std::vector<std::string>::iterator &it, ServerConfig &server);
		void						handleErrorPage(std::vector<std::string>::iterator &it, ServerConfig &server);
		void						handleMaxBodySize(std::vector<std::string>::iterator &it, ServerConfig &server);
		void						handleRoot(std::vector<std::string>::iterator &it, ServerConfig &server);

		// HANDLERS location
		void						handleMethods(std::vector<std::string>::iterator &it, LocationConfig &location);
		void						handleAutoindex(std::vector<std::string>::iterator &it, LocationConfig &location);
		void						handleIndex(std::vector<std::string>::iterator &it, LocationConfig &location);
		void						handleReturn(std::vector<std::string>::iterator &it, LocationConfig &location);
		void						handleUploadStore(std::vector<std::string>::iterator &it, LocationConfig &location);
		void						handleCgi(std::vector<std::string>::iterator &it, LocationConfig &location);
		void						handleRoot(std::vector<std::string>::iterator &it, LocationConfig &location);

		// check					
		void						checkSemicolon(std::vector<std::string>::iterator &it);
		void						checkBracketsBalance(const std::string &content);
		int							stringToInt(const std::string &str); // equivalent de stoi
		size_t						parseSize(const std::string &str); // pour parser les tailles avec K, M, G

		// verif
		void 						verifyConfig(); // verifie que chaque serveur a au moins un port et une location, et qu'il n'y a pas de doublons de ports

	public:
		ParserConfig();
		ParserConfig(const ParserConfig &other);
		ParserConfig &operator=(const ParserConfig &other);
		~ParserConfig();

		void parse(const std::string &configFilePath); // fonction publique principqle

		// Getters pour récupérer les serveurs après parsing
    	const std::vector<ServerConfig> &getServers() const { return _servers; }

		// classe erreur pour les erreurs de parsing
		class ParseException : public std::exception {
			private:
				std::string _msg;

			public:
				ParseException(const std::string &msg) : _msg(msg) {}

				virtual ~ParseException() throw() {}

				const char* what() const throw() {
					return _msg.c_str();
				}
		};
};


#endif

// Faire une classe config parser qui va:
//  lire le fichier de config, 
// le parser 
// et stocker les infos dans des strucs ServerConfig et LocationConfig


// ds obligatoire :
// listen
// root
// location 

// si y'a pas le minimum -> error 
// et sinon on met en defaut le reste 
// ou alors on cree l'objet avec tout a defaut 
// et on rempli avec le parsing


// faire pointeurs sur fonction pour eviter les foret de if 

// faire tout un tas de handle . Par ex : handle_listen, handle_root .... 
// et ne pas oublier a la fin du handle de verif si c'est la fin de directive ;

/*
 * void ConfigParser::_handleListen(std::vector<std::string>::iterator &it) {
    // 1. Convertir *it en int (le port)
    // 2. it++;
    // 3. Vérifier si *it == ";"
    if (*it != ";") throw std::runtime_error("';' manquant après le port");
}
 */

// a la fin du parsing verifier que chaque serveur a au moions un port et une location 
// et verif les doublon, 2 srveurs qui ecoutent sur le meme port 