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

	LOG("\n>>> METHOD GET"); // ---------------------------------

    const LocationConfig* location = findMatchingLocation(client);
    std::string path = client->getRequest().getPath();

    if (isCGI(location, path)) {
        LOG(">>> CGI DETECTED");
        return executeCGI(client, location, path);
    }

	//std::string path = client->getRequest().getPath();
	LOG("Resolved path: " << path); // ---------------------------------

	struct stat st;
	if (stat(path.c_str(), &st) < 0){
		LOG("Calling stat on: " << path); // -----------------------
		return buildErrorResponse(404, client);
	}
	//3. Permission
	if (access(path.c_str(), R_OK) != 0) {
		return buildErrorResponse(403, client);
	}
	// 4. Directory → gérer index / autoindex
	if (st.st_mode & S_IFDIR){
		LOG("IS DIRECTORY"); // ---------------------------------
		return handleDirectory(client, path);
	}
	//5. File
	LOG("Serving file: " << path); // ---------------------------
	return serveFile(client, path, st);

}

HttpResponse	WebServer::methodPost(SocketClient* client){
	LOG("\n>>> METHOD POST "); // -------------------------------------------------------------
	(void)client;

	const LocationConfig* location = findMatchingLocation(client);
	if (!location)
		return buildErrorResponse(404, client);

	std::string path = client->getRequest().getPath(); //mettre en const si ca hurle (url)

	if (isCGI(location, path)){
		LOG("on part pour executer le CGI"); // -------------------------------------------------------------
		return (executeCGI(client, location, path));
	}
	else {
		LOG("Ca c est du static"); // -------------------------------------------------------------
		return (executeStatic(client, path));
	}
}

HttpResponse WebServer::methodDelete(SocketClient* client)
{
    std::string path = client->getRequest().getPath();

    if (path.find("/upload/") == std::string::npos)
        return buildErrorResponse(403, client);

    if (path.find("..") != std::string::npos)
        return buildErrorResponse(403, client);

    std::string dir = path.substr(0, path.find_last_of('/'));

    if (access(dir.c_str(), W_OK) == -1)
        return buildErrorResponse(403, client);

    struct stat st;
    if (stat(path.c_str(), &st) < 0)
        return buildErrorResponse(404, client);

    if (!S_ISREG(st.st_mode))
        return buildErrorResponse(403, client);

    if (std::remove(path.c_str()) != 0)
        return buildErrorResponse(500, client);

    HttpResponse res;
    res.statusLine = "HTTP/1.1 204 No Content";
    return res;
}



