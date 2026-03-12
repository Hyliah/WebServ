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

#ifndef LOCATIONCONFIG_HPP
#define LOCATIONCONFIG_HPP

#include "Webserv.hpp"

// struc location config ? 
class LocationConfig {
public:
    std::string             path;          // le chemin Ex: "/something"
    std::string             root;          // Dossier physique
    std::vector<std::string> methods;       // GET, POST, DELETE
    bool                    autoindex;     // si on ou off pour savoir sin on affiche le listing du dossier ou pas
    std::string             index;         // Fichier par défaut (index.html)
    std::string             return_url;    // Pour les redirections (301) - a checker 
    std::string             upload_store;  // Dossier pour les uploads

	// ca c'est poru le CGI j'ai toujours pas capté mais apprently ca ressemble a ca :
    std::map<std::string, std::string> cgi_info; // Extension -> Chemin de l'exécutable

    LocationConfig() : autoindex(false) {}
};

#endif
