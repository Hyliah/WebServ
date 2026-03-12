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

#include "ParserConfig.hpp"


// clean:
// retirer commentaire (#) et space inutiles

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

