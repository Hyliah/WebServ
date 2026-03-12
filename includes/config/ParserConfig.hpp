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
#include "ServerConfig.hpp"

class ParserConfig {

	public:
		std::vector<ServerConfig> 	server;
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


// methode remove comments 

// methode tokenize 

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