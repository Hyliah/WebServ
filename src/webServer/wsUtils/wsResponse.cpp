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

#include "WebServer.hpp"


/* ************************************************** */
/* UTILS RESPONSE.     		                          */
/* ************************************************** */

void	WebServer::methodGet(){

	//1. resolve path (faire avant pendant le parsing) /definir taille max URI - 414
	if (client._request.getUri() > MAX_URI_SIZE) // a mettre dans la verif au parsing
		// return 414;
	std::string path = resolvePath();

	//2. Vérifications sécurité (path traversal, etc.)

	//3. Vérifier existence (stat)
	struct stat st;
	int	statReturn;
	
	statRetrun = stat(path.c_str(), &st);

	if (statRetrun < 0) {
		// 404 Not Found
	}
	
	//4. Vérifier permissions
	if (access(path.c_str(), R_OK) != 0) {
		// 403 Forbidden
	}
	
	if (statRetrun == 0){
		// 5. Si dossier → gérer index / autoindex
		if (st.st_mode & S_IFDIR){
			handleDirectory(path);
		}
		//6. Lire fichier
		else
			std::ifstream file(path.c_str());
		

	}
	
	/*
	7. Construire réponse HTTP + headers
	8. Envoyer (send)
	9. Gérer connexion (keep-alive ou close)
	*/
	
	/*

	- lire le fichier ? URI
	- générer le body 
	- creation d une http response -> stringstream 
	- ajout des headers
	- fonction send : send(fd, response.c_str(), response.size(), 0);
	- closeConnection(fd);
	*/
}



void	WebServer::methodPost(){

}

void	WebServer::methodDelete(){

}

