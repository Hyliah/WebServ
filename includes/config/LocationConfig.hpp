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

#include <string>
#include <vector>
#include <map>

class LocationConfig {
public:
	std::string				path;          // le chemin Ex: "/something"
	std::string				root;          // Dossier physique

	std::vector<std::string> methods;       // GET, POST, DELETE ( maybe bool pour savoir si c'est la ou pas ?)

	bool					autoindex;     // si on ou off pour savoir sin on affiche le listing du dossier ou pas
	std::vector<std::string> index;         // Fichier par défaut (index.html)

	std::string				returnUrl;    // Pour les redirections (301) - a checker 

	std::string				uploadStore;  // Dossier pour les uploads

	size_t					maxBodySize;

	std::map<std::string, std::string> cgiInfo; // Extension -> Chemin de l'exécutable

	bool					cgiEnabled;

	bool					hasGet;
	bool					hasPost;
	bool					hasDelete;

	LocationConfig();
    LocationConfig(const LocationConfig& other);
    LocationConfig& operator=(const LocationConfig& other);
    ~LocationConfig();

};

#endif
