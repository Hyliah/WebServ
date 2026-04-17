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

std::string WebServer::resolvePath(const SocketClient* client) {
    const std::string& uri = client->getRequest().getUri();
    int fd = client->getFd();

    size_t pos = uri.find('?');
    std::string path = uri.substr(0, pos);
	//resolveQuery(client, uri.substr(pos, uri.end()));

    std::string finalPath = decodePath(path, fd);
    finalPath = normalizePath(finalPath, fd);
    checkErrorPath(finalPath, fd);
	std::string root = findRoot(client);

	cleanFinalPath(root, finalPath);
    finalPath = root + finalPath;

    return finalPath;
}

std::string	WebServer::decodePath(const std::string &path, int fd){
	std::string output;

	if (path.empty())
		throw ResponseException(fd, 400);

	for (size_t i = 0 ; i < path.size() ; i++){
		if (path[i] == '%') {
			if (i + 2 >= path.size())
				throw ResponseException(fd, 400);

			char c1 = path[i + 1];
			char c2 = path[i + 2];

			if (c1 == '0' && c2 == '0')
				throw ResponseException(fd, 400);

			if (!isHex(c1) || !(isHex(c2)))
				throw ResponseException(fd, 400);

			char decoded = hexToChar(c1, c2);

			output += decoded;
			i += 2;
		}
		// else if (path[i] == '+')
        //     output += ' ';
		else 
			output += path[i];
	}

	if (output.find("%2f") != std::string::npos)
		throw ResponseException(fd, 403);
	
	if (output.find('\0') != std::string::npos)
		throw ResponseException(fd, 400);
	
	return output;
}

std::string	WebServer::normalizePath(const std::string &path, int fd){

	std::vector<std::string> segmentPath;
	std::stringstream ss(path);
	std::string segment;

	while((getline(ss, segment, '/'))){
		if (segment == "" || segment == ".")
			continue;
		
		else if (segment == ".."){
			if (segmentPath.empty())
				throw ResponseException(fd, 400);
			segmentPath.pop_back();
		}

		else 
			segmentPath.push_back(segment);
	}

	std::string result = "/";
	for (size_t i = 0 ; i < segmentPath.size() ; i++){
		result += segmentPath[i];
		if (i != segmentPath.size() - 1)
			result += "/";
	}

	return result;
}

void WebServer::cleanFinalPath(std::string& root, std::string& finalPath) {
    if (root.empty())
        return;

    if (finalPath.empty()) {
        finalPath = "/";
        return;
    }

    if (root[root.size() - 1] == '/' && finalPath[0] == '/')
        finalPath = finalPath.substr(1);

    else if (root[root.size() - 1] != '/' && finalPath[0] != '/')
        root += "/";
}

std::string WebServer::findRoot(const SocketClient* client){
	const std::vector<const ServerConfig*>& conf = client->getServer()->getServers();
    const HttpRequest& req = client->getRequest();
    const std::map<std::string, std::string>& headers = req.getHeaders();

    std::string host;
    std::map<std::string,std::string>::const_iterator it = headers.find("host");
    if (it != headers.end())
        host = it->second;

    std::string recupRoot;
    for (size_t i = 0; i < conf.size(); ++i) {
        const ServerConfig* cfg = conf[i];
        if (cfg->serverName == host) {
            recupRoot = cfg->root;
            break;
        }
    }
    if (recupRoot.empty() && !conf.empty())
        recupRoot = conf[0]->root;

	return recupRoot;
}

//voir d ou elle part pour voir comment on renvoi l erreur : buildErrorResponse(403);
void	WebServer::checkErrorPath(const std::string &path, int fd){
	if (path.find("%") != std::string::npos)
		throw ResponseException(fd, 403);

	if (path.find("//") != std::string::npos)
		throw ResponseException(fd, 403);

	if (path.find("..") != std::string::npos)
		throw ResponseException(fd, 403);

	for (size_t i = 0 ; i < path.size() ; i++){
		if (path[i] < ' ' && path[i] > '~')
			throw ResponseException(fd, 403);
	}
}


void	WebServer::resolveQuery(const SocketClient* client, std::string path){
	
	std::map<std::string, std::string> query;

	int posStart = 0;

	while (posStart < path.length()){
		
		size_t posEnd = path.find('&', posStart);
		
		if (posEnd == std::string::npos){
			posEnd = path.length();
		}

		std::string chunk = path.substr(posStart, posEnd - posStart);

		size_t equalPos = chunk.find('=');

		std::string key;
		std::string value;

		if (equalPos == std::string::npos){ 
			key = chunk;
			value = "";
		}

		key = chunk.substr(equalPos);
		value = chunk.substr(equalPos + 1);

		key = UrlDecode(client, key);
		value = UrlDecode(client, value);

		if (!key.empty())
			query[key] = value;

		posStart = posEnd + 1;

	}
					
	client->getRequest().setQuery = query;


	//test?key1=val1&key2=val2 -> key1 = "val1"     key2 = "val2"
	//test?key1=&key2=val2 -> key1 = ""     key2 = "val2"
	//test?key1 -> key1 = ""
	//test?key1=val1=key2=val2 -> key1 = "val1=key2=val2"
	
	//si 2x les meme on garde le 2e
	// %20 -> espace
	// + -> espace 
	// faire gestion generale des % par contre si pas hexa -> badrequest -> hexToChar(dejac codee) 
	// si \0 bad request

}

std::string WebServer::UrlDecode(const SocketClient *client, std::string entry){
	std::string result;

	for (size_t i = 0; i < entry.length(); i++){

		if (entry[i] == '%"'){
			if (i + 2 >= entry.size())
				throw ResponseException(client->getFd(), 400);

			char c1 = entry[i + 1];
			char c2 = entry[i + 2];

			if (c1 == '0' && c2 == '0')
				throw ResponseException(client->getFd(), 400);

			if (!isHex(c1) || !(isHex(c2)))
				throw ResponseException(client->getFd(), 400);

			char decoded = hexToChar(c1, c2);

			result += decoded;
			i += 2;
		}

		else if (entry[i] == '+'){
			result += " ";
		}

		else {
			result += entry[i];
		}

	}
}