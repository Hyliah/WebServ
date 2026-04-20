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

HttpResponse	WebServer::methodGet(SocketClient* client){

	LOG(">>> methodGet");

	//1. Max URI
	//2. Résolution du Path
	// changé pour mettre a l 'étape d avant car on l utilise pas que pour le get :)
	
	std::string path = client->getRequest().getPath();

	LOG("Resolved path: " << path);

	struct stat st;
	if (stat(path.c_str(), &st) < 0){
		LOG("Calling stat on: " << path);
		return buildErrorResponse(404, client);
	}
	//3. Permission
	if (access(path.c_str(), R_OK) != 0) {
		return buildErrorResponse(403, client);
	}
	
	// 4. Directory → gérer index / autoindex
	if (st.st_mode & S_IFDIR){

		LOG("IS DIRECTORY");
		
		return handleDirectory(client, path);
	}
	
	//5. File
	return serveFile(client, path, st);

	LOG("Serving file: " << path);
	
	/*
	- fonction send : send(fd, response.c_str(), response.size(), 0);
	- closeConnection(fd);
	*/
}

HttpResponse	WebServer::methodPost(SocketClient* client){
	(void)client;

// 	const LocationConfig* location = findMatchingLocation(client);
// 	if (!location)
// 		return buildErrorResponse(404, client);

// 	std::string path = client->getRequest().getPath(); //mettre en const si ca hurle (url)


// 	if (isCGI(location, path))
// 		return (executeCGI(client, location, path));
// 	else
// 		return (executeStatic(client, location, path));

// 	// defini si on traite en cgi ou non
// 		// if CGI
// 		// else
	

// // A FAIRE



	HttpResponse res;
	return res;
}

HttpResponse	WebServer::methodDelete(SocketClient* client){
	(void)client;
	HttpResponse res;
	return res;
// A FAIRE
}

