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
/* Directory					                      */
/* ************************************************** */

Response WebServer::handleDirectory(SocketClient& client, const std::string& path){

	const ServerConfig* config = findMatchingConfig(client);
	
	if (!config->index.empty()){
		for (size_t i = 0 ; i < config->index.size() ; i++){
			std::string fullPath = path;
			
			if (fullPath[fullPath.size() - 1] != '/')
        		fullPath += "/";

    		fullPath += config->index[i];

			struct stat st;
			if (stat(fullPath.c_str(), &st) == 0) {
                if (access(fullPath.c_str(), R_OK) == 0)
                    return serveFile(client, fullPath, st);
            }
		}
	}

	if (config->autoindex == true){
		return generateListing(path);
			// générer le html de la liste des fichiers
	}

	//return 403;
}

Response WebServer::serveFile(SocketClient& client, const std::string& path, struct stat& st){
	Response res;
	std::ifstream file(path.c_str(), std::ios::binary); //chercher wtf
	if (!file)
	// return 500 (faire fonction qui retourne une response et prend en parame le code d erreur)
	
	std::ostringstream ss;
	ss << file.rdbuf();
	std::string body = ss.str();

	return fillResponseOK(body, body.size(), getMimeType(path));
}

Response	WebServer::generateListing(const std::string &path){
	DIR* dir = opendir(path.c_str());
	if (!dir)
		//erreur 403 ou 500 

	std::stringstream body;

	body << "<html><head><title>Index of " << path << "</title></head><body>";
	body << "<h1>Index of " << path << "</h1><hr><ul>";

	struct dirent* entry;

	while ((entry = readdir(dir)) != NULL) {
		std::string name = entry->d_name;
		if (name == "." || name == "..") continue;

		body << "<li><a href=\"" << name;
        if (entry->d_type == DT_DIR)
            body << "/";
        body << "\">" << name << "</a></li>";
	}
	
	body << "</ul><hr></body></html>";

	closedir(dir);

	std::string content = body.str();
	return fillResponseOK(content, content.size(), "text/html");
}

Response	WebServer::fillResponseOK(std::string body, long size, std::string type){
	Response res;

	res.statusLine = "HTTP/1.1 200 OK";
	res.body = body;
	res.headers["Content-Length"] = longToString(size);
	res.headers["Content-Type"] = type;
	res.headers["Connection"] = "close";

	return res;
}

const ServerConfig* WebServer::findMatchingConfig(SocketClient& client) {
    const std::vector<const ServerConfig*>& configs = client.getServer().getServers();

    std::string host;
    const std::map<std::string, std::string>& headers = client.getRequest().getHeaders();

    std::map<std::string, std::string>::const_iterator it = headers.find("host");
    if (it != headers.end())
        host = it->second;

    for (size_t i = 0; i < configs.size(); i++) {
        if (configs[i]->serverName == host)
            return configs[i];
    }

    if (!configs.empty())
        return configs[0];

    return NULL;
}


/* ************************************************** */
/* File							                      */
/* ************************************************** */






//Response WebServer::buildErrorResponse(int code);