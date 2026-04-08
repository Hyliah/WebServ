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

std::string WebServer::resolvePath() {
    std::string& uri = client._request.getUri();
    
    size_t pos = uri.find('?');
    std::string path = uri.substr(0, pos);

    std::string finalPath = decodePath(path);
    finalPath = normalizePath(finalPath);
    checkErrorPath(finalPath);
	std::string root = findRoot();

	cleanFinalPath(root, finalPath);
    finalPath = root + finalPath;

    return finalPath;
}

std::string	WebServer::decodePath(const std::string &path){
	std::string output;

	if (path.empty())
		//error bitches 400
	for (size_t i = 0 ; i < path.size() ; i++){
		if (path[i] == '%') {
			if (i + 2 >= path.size())
				//error bitches 400
			char c1 = path[i + 1];
			char c2 = path[i + 2];

			if (c1 == '0' && c2 == '0')
				//error bitches 400

			if (!isHex(c1) || !(isHex(c2)))
				//error bitches 400
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
		//error bitches 403 
	
	if (output.find('\0') != std::string::npos)
    	// throw 400;
	
	return output;
}

std::string	WebServer::normalizePath(const std::string &path){

	std::vector<std::string> segmentPath;
	std::stringstream ss(path);
	std::string segment;

	while((getline(ss, segment, '/'))){
		if (segment == "" || segment == ".")
			continue;
		
		else if (segment == ".."){
			if (segmentPath.empty())
				//error bitches 400 
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

std::string WebServer::findRoot(){
	const std::vector<const ServerConfig*>& conf = client.getServer().getServers();
    HttpRequest& req = client.getRequest();
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

void	WebServer::checkErrorPath(const std::string &path){
	if (output.find("%") != std::string::npos)
		//error bitches 403 
	if (output.find("//") != std::string::npos)
		//error bitches 403 
	if (output.find("..") != std::string::npos)
		//error bitches 403 
	for (size_t i = 0 ; i < path.size() ; i++){
		if (path[i] < ' ' && path[i] > 176)
			//error bitches 403 
	}
}
