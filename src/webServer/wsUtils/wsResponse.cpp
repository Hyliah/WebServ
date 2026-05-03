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

HttpResponse	WebServer::methodGet(SocketClient* client, const LocationConfig* location){

	LOG("\n>>> METHOD GET"); // ---------------------------------

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

HttpResponse	WebServer::methodPost(SocketClient* client, const LocationConfig* location){
	LOG("\n>>> METHOD POST "); // -------------------------------------------------------------

	std::string path = client->getRequest().getPath();

	if (path.find("..") != std::string::npos){
		//LOG("THE BRIT BRIT"); // -------------------------------------------------------------
		return buildErrorResponse(403, client);
	}

	if (isCGI(location, path)){
		//LOG("on part pour executer le CGI"); // -------------------------------------------------------------
		return (executeCGI(client, location, path));
	}
	
	if (path.find("upload") == std::string::npos){
		//LOG("BOYS BOYS BOYS"); // -------------------------------------------------------------
		return buildErrorResponse(403, client);
	}
	else {
		//LOG("Ca c est du static"); // -------------------------------------------------------------
		return (executeStatic(client, path));
	}
}

HttpResponse WebServer::methodDelete(SocketClient* client, const LocationConfig* location)
{
	LOG("\n>>> METHOD DELETE "); // -------------------------------------------------------------
    
	std::string path = client->getRequest().getPath();
	//LOG("\n>>> path a til un super / ?? " << path);
	//LOG("la query bitches");
	
	//CGI
	if (isCGI(location, path))
    	return executeCGI(client, location, path);
	//PAS CGI
	else {
		if (path.find("/upload/") == std::string::npos){
			//LOG("ULPOAAAAD------------------------");
			return buildErrorResponse(403, client);
		}

		if (path.find("..") != std::string::npos){
			return buildErrorResponse(403, client);
		}

		std::string dir = path.substr(0, path.find_last_of('/'));

		// if (access(dir.c_str(), W_OK) == -1)
		// 	return buildErrorResponse(403, client);

		struct stat st;
		if (stat(path.c_str(), &st) < 0)
			return buildErrorResponse(404, client);

		if (!fullDelete(path)) {
			return buildErrorResponse(500, client);
		}

		HttpResponse res;
		res.statusLine = "HTTP/1.1 204 No Content";
		return res;
	}
}

bool WebServer::fullDelete(const std::string& path) {
    struct stat st;

    if (stat(path.c_str(), &st) != 0) {
        return false;
    }

    if (S_ISDIR(st.st_mode)) {
        DIR* dir = opendir(path.c_str());
        if (!dir) return false;

        struct dirent* entry;
        while ((entry = readdir(dir)) != NULL) {
            std::string name = entry->d_name;

            if (name == "." || name == "..") continue;

            std::string fullPath = path;
            if (path[path.length() - 1] != '/') {
                fullPath += "/";
            }
            fullPath += name;

            if (!fullDelete(fullPath)) {
                closedir(dir);
                return false;
            }
        }
        closedir(dir);
        return (std::remove(path.c_str()) == 0);
    } 
    
    else {
        return (std::remove(path.c_str()) == 0);
    }
}

HttpResponse WebServer::buildRedirectResponse(int returnCode, const std::string& url, SocketClient* client){
	LOG("\n>>> REDIRECT RESPONSE "); // -------------------------------------------------------------
	HttpResponse res;

	std::string status;
	if (returnCode == 301)	status = "Moved Permanently";
	else if (returnCode == 302)	status = "Found";
	else if (returnCode == 307)	status = "Temporary Redirect";
	else if (returnCode == 308)	status = "Permament Redirect";
	else status = "Redirect";

	res.statusLine = "HTTP/1.1 " + longToString((long)returnCode) + " " + status; //voir avec ces histoire de long si ca joue pas
	res.headers["location"].push_back(url);
	res.headers["content-length"].push_back("0");

	if (client->keepAlive)
		res.headers["connection"].push_back("keep-alive");
	else
		res.headers["connection"].push_back("close");

	return res;
}
