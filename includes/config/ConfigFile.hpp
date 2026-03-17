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

#ifndef CONFIGFILE_HPP
#define CONFIGFILE_HPP


// JE NE SAIS PAS SI ON VA UTILISER CE FICHIER
// JE PENSAIS Y METTRE DES TRUCS QUI ONT BOUGÉS DONC TBC FOR NOW 

// creer une classe config avec un vector pour stocker les objets cree pdt le parsing ??
// check si des autrres trucs a mettre

// UNe fois le parsing fini, on passe std::vector<ServerConfig> a une classe serverManager par exemple ?? 
// renommer ce fichier 
// la classe va :
// 1 - parcourir chaque config
// 2 - creer un socket pour chaque port unique trouvé
// 3 - lancer la boucle poll() ??
// ... etc ?? 

// ex : 
// class ServerManager
// {
//		std::vector<ServerManager> servers;
// };


#endif
