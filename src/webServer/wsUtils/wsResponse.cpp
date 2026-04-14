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
	if (client->getRequest().getUri().size() > MAX_URI_SIZE) // a mettre dans la verif au parsing
		return buildErrorResponse(414);
	
	LOG("URI: " << client->getRequest().getUri());
	
	//2. Résolution du Path
	std::string path = resolvePath(client);

	LOG("Resolved path: " << path);

	struct stat st;
	if (stat(path.c_str(), &st) < 0){
		LOG("Calling stat on: " << path);
		return buildErrorResponse(404);
	}
	//3. Permission
	if (access(path.c_str(), R_OK) != 0) {
		return buildErrorResponse(403);
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



HttpResponse	WebServer::methodPost(){
	HttpResponse res;
	return res;
// A FAIRE
}

HttpResponse	WebServer::methodDelete(){
	HttpResponse res;
	return res;
// A FAIRE
}

