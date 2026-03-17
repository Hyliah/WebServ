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

	// check si on fait juste 2 string 
	// std::string                 cgi_path;
	// std::string                 cgi_extension;

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


// Pourquoi a la place de faire une fonction init, au moment de la creation de l'objet on rempli pas toute la classe dans le constructeur ? 
// Par ex :
// LocationConfig() : path("default"), root("default"), index("default"), returnUrl("default"), uploadStore("default"),
//  autoindex(false), cgiEnabled(false) {}
// ou une presentation un peu mieux comme 
/*
 * LocationConfig::LocationConfig()
{
	path = "default";
	root = "default";
	index = "default";
	returnUrl = "default";
	uploadStore = "default";
	autoindex = false;
	cgiEnabled = false;
}
 * 
 */

 // update j'ai demandé a Chatty :
 /*
  * si creation d'objet et init apres :
  L’objet peut exister dans un état invalide
  Entre la création et l’appel de initLocationConfig, l’objet n’est pas initialisé.
  bug potentiel
  fonction supplmentaire inutile
  * 
  * si init dans le constructeur :
  * objet toujorus valdie
  * moins de code
  * meilleure performances 
  * 
  * globalement il faut:
  * creer l'objet avec valeur par defaut ds le constructeur
  * ensuite elle sont modifiee par le parser comme convenu
  */