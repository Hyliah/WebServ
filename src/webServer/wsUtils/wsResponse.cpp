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

	struct stat st;
	if (stat(path.c_str(), &st) < 0){
		return buildErrorResponse(404, client);
	}

	if (access(path.c_str(), R_OK) != 0) {
		return buildErrorResponse(403, client);
	}

	if (st.st_mode & S_IFDIR){
		return handleDirectory(client, path);
	}

	return serveFile(client, path, st);

}

HttpResponse	WebServer::methodPost(SocketClient* client, const LocationConfig* location){
	LOG("\n>>> METHOD POST "); // -------------------------------------------------------------

	std::string path = client->getRequest().getPath();

	if (isCGI(location, path)){
		return (executeCGI(client, location, path));
	}
	
	if (path.find("upload") == std::string::npos){
		return buildErrorResponse(403, client);
	}
	else {
		return (executeStatic(client, path));
	}
}

HttpResponse WebServer::methodDelete(SocketClient* client, const LocationConfig* location){
	LOG("\n>>> METHOD DELETE "); // -------------------------------------------------------------
    
	std::string path = client->getRequest().getPath();

	if (isCGI(location, path))
    	return executeCGI(client, location, path);

	else {
		if (path.find("/upload/") == std::string::npos){
			return buildErrorResponse(403, client);
		}

		std::string dir = path.substr(0, path.find_last_of('/'));

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

	res.statusLine = "HTTP/1.1 " + longToString((long)returnCode) + " " + status; 
	res.headers["location"].push_back(url);
	res.headers["content-length"].push_back("0");

	if (client->keepAlive)
		res.headers["connection"].push_back("keep-alive");
	else
		res.headers["connection"].push_back("close");

	return res;
}
