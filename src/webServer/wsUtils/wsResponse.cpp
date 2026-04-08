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

HttpResponse	WebServer::methodGet(const SocketClient* client){

	//1. Max URI
	if (client->getRequest().getUri().size() > MAX_URI_SIZE) // a mettre dans la verif au parsing
		;	
		// return 414;
	
	//2. Résolution du Path
	std::string path = resolvePath(client);

	struct stat st;
	if (stat(path.c_str(), &st) < 0)
		;
		// return error 404

	//3. Permission
	if (access(path.c_str(), R_OK) != 0) {
		;
		// return builderrr(403) Forbidden
	}
	

	// 4. Directory → gérer index / autoindex
	if (st.st_mode & S_IFDIR){
		return handleDirectory(client, path);
	}
	
	//5. File
	return serveFile(client, path, st);
	
	
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

