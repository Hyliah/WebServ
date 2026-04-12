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
#include "../utils/utilsParsing.hpp"


/* ************************************************** */
/* Directory					                      */
/* ************************************************** */

HttpResponse WebServer::handleDirectory(const SocketClient* client, const std::string& path){

	const ServerConfig* config = findMatchingConfig(client);
	const LocationConfig* loc = findMatchingLocation(client);

	const std::vector<std::string>* indexes;
	bool autoindex;

	if (loc && !loc->index.empty())
		indexes = &loc->index;
	else
		indexes = &config->index;

	if (loc)
		autoindex = loc->autoindex;
	else
		autoindex = false;

	// INDEX
    if (!indexes->empty()) {
        for (size_t i = 0; i < indexes->size(); i++) {
            std::string fullPath = path;

            if (!fullPath.empty() && fullPath[fullPath.size() - 1] != '/')
                fullPath += "/";

            fullPath += (*indexes)[i];

            struct stat st;
            if (stat(fullPath.c_str(), &st) == 0 && S_ISREG(st.st_mode)) {
                if (access(fullPath.c_str(), R_OK) == 0)
                    return serveFile(client, fullPath, st);
            }
        }
    }

    // AUTOINDEX
    if (autoindex) {
        return generateListing(path);
    }

    // FORBIDDEN
    return buildErrorResponse(403);
}



std::string getMimeType(std::string path){
	(void)path;
	return "hello";
}
HttpResponse WebServer::serveFile(const SocketClient* client, const std::string& path, struct stat& st){
	(void)client;
	(void)st;
	
    std::ifstream file(path.c_str(), std::ios::binary);
    if (!file) {
        return buildErrorResponse(500);
    }
	
	std::ostringstream ss;
	ss << file.rdbuf();
	std::string body = ss.str();

	HttpResponse res = fillResponseOK(body, body.size(), getMimeType(path));
	return res;
}



HttpResponse	WebServer::generateListing(const std::string &path){
	DIR* dir = opendir(path.c_str());
	if (!dir)
		return buildErrorResponse(403);

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

HttpResponse	WebServer::fillResponseOK(std::string body, long size, std::string type){
	HttpResponse res;

	res.statusLine = "HTTP/1.1 200 OK";
	res.body = body;
	res.headers["Content-Length"] = longToString(size);
	res.headers["Content-Type"] = type;
	res.headers["Connection"] = "close"; //keep-alive

	return res;
}

const ServerConfig* WebServer::findMatchingConfig(const SocketClient* client) const {
    const std::vector<const ServerConfig*>& configs = client->getServer()->getServers();

    std::string host;
    const std::map<std::string, std::string>& headers = client->getRequest().getHeaders();

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

const LocationConfig* WebServer::findMatchingLocation(const SocketClient* client) {
    const ServerConfig* server = findMatchingConfig(client);
    if (!server)
        return NULL;

    const std::string& uri = client->getRequest().getUri();

    const LocationConfig* bestMatch = NULL;
    size_t bestLen = 0;

    for (size_t i = 0; i < server->locations.size(); ++i) {
        const LocationConfig& loc = server->locations[i];
        const std::string& locPath = loc.path;

        if (locPath.empty())
            continue;

        if (uri.compare(0, locPath.size(), locPath) != 0)
            continue;

        if (uri.size() > locPath.size()) {
            if (locPath[locPath.size() - 1] != '/' &&
                uri[locPath.size()] != '/')
                continue;
        }

        if (locPath.size() > bestLen) {
            bestMatch = &loc;
            bestLen = locPath.size();
        }
    }

    return bestMatch;
}

/* ************************************************** */
/* File							                      */
/* ************************************************** */


