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
#include "../utils/utilsGeneral.hpp"

bool WebServer::isCGI(const LocationConfig* location, const std::string& path){
    (void)location;
    (void)path;
    //coder un truc

    //aller chercher dans le parsinf du fichier de conf de la loc si on a des formats correspondant
    // depend soit d une extension soit d un flag en config. Faut il y avoir les 2 ou tout fonctionne ? 
    return true;
}

HttpResponse	WebServer::executeCGI(const SocketClient* client, const LocationConfig* location, const std::string& path) {
    if (access(path.c_str(), F_OK) == -1)
        return buildErrorResponse(404, client);
    
    if (access(path.c_str(), X_OK) == -1)
        return buildErrorResponse(403, client);
    
    int bodyFd = open(client->getRequest().getBodyPath().c_str(), O_RDONLY);
    if (bodyFd == -1)
        return buildErrorResponse(500, client);

    std::map<std::string, std::string> map = createEnvp(client, location, path);
    char** envp = convertMapToChar(map);

    int pipeFd[2] = {-1};
    
    if (pipe(pipeFd) == -1){
        freeTab(&envp);
        return buildErrorResponse(500, client);
    }

    pid_t pid = fork();
    if (pid == -1){
        safeClose(&pipeFd[0]); safeClose(&pipeFd[1]);
        freeTab(&envp);
        return buildErrorResponse(500, client);
    }

    if (pid == 0){
        dup2(bodyFd, STDIN_FILENO);
        dup2(pipeFd[1], STDOUT_FILENO);

        safeClose(&bodyFd);
        safeClose(&pipeFd[0]); safeClose(&pipeFd[1]);

        char* argv[] = { const_cast<char*>(path.c_str()), NULL }; // creation d un tableau *[] pour mettre en tab[0] le paht et tab[1] NULL
        execve(path.c_str(), argv, envp);
        
        exit(1);
    }

    close(bodyFd);
    close(pipeFd[1]);
    
    int status;
    time_t start = time(NULL);
    const int MAX_WAIT = 5;

    while (true){
        pid_t result = waitpid(pid, &status, WNOHANG);
        if ( result = pid)
            break;
        if (difftime(time(NULL), start) > MAX_WAIT)
        {
            kill(pid, SIGKILL);
            close(pipeFd[0]);
            freeTab(&envp);
            return buildErrorResponse(504, client);
        }
        usleep(1000);
    }
    
    std::string output;
    char buffer[4096];
    ssize_t bytes;

    while ((bytes = read(pipeFd[0], buffer, sizeof(buffer))) > 0)
        output.append(buffer, bytes);

    close(pipeFd[0]);
    freeTab(&envp);
    
    return createCGIResponse(client, output);
}

HttpResponse	WebServer::createCGIResponse(const SocketClient* client, std::string raw){
    HttpResponse res;

    size_t pos = raw.find("\r\n\r\n");
    size_t sep_len = 4;

    if (pos == std::string::npos)
    {
        pos = raw.find("\n\n");
        sep_len = 2;
    }

    if (pos == std::string::npos)
        return buildErrorResponse(500, client);

    std::string headers = raw.substr(0, pos);
    std::string body = raw.substr(pos + sep_len);

    std::vector<std::string> lines = splitLines(headers); //coder splitlines bibi

    for (size_t i = 0; i < lines.size(); i++)
    {
        if (lines[i].empty())
            continue;

        size_t colon = lines[i].find(':');
        if (colon == std::string::npos)
            continue;

        std::string key = trim(lines[i].substr(0, colon));
        std::string value = trim(lines[i].substr(colon + 1));

        if (key == "Status"){
            res.statusLine = atoi(value.c_str());
        }
        else{
            res.headers[key] = value;
        }
    }

    if (res.statusLine.empty())
        res.statusLine = 200;

    res.body = body;
    
    return res;
}

HttpResponse	WebServer::executeStatic(const SocketClient* client, const LocationConfig* location, const std::string& path) {
    (void)client;
    (void)location;
    (void)path;
   
    //faire des verif de chmod ici ou ailleurs ??? -> 405 acccess denied ???
    //ou checker si post dans method ?

    // verifier taille max again ?

    // defnir le content type via le header du client
    // contentType = client.getRequest().getHeaders(["Content-Type"])

    //if MULTIPART : (contentType contient "multipart/form-data") -> "juste" apload un truc -> return handleUnFIle() - 201
    //else if URLENCODED : (contentType == "application/x-www-form-urlencoded") -> parserUrl -> return handleTrucParse() -> httpResponse - 201
    //else if JASON : contentType == "application/json" -> parseJason -> return handleTrucParse() -> HttpResponse - 201
    //else return 415 unsupported media part 
    
    HttpResponse res;
    return res;
}



const std::map<std::string, std::string> WebServer::createEnvp(const SocketClient* client, const LocationConfig* location, const std::string& path){
    
    (void)client;
    (void)location;
    (void)path;
    
    std::map<std::string, std::string> map;

    // creer un tableau avec les infos pour que ce soit transformer en 2e partie en char** pour le exeve minishell way

    // recuperer la request dans le client

    // env["REQUEST_METHOD"] = request.method
    // env["QUERY_STRING"] = request.query   // ?a=1&b=2
    // env["CONTENT_LENGTH"] = request.headers["Content-Length"]
    // env["CONTENT_TYPE"] = request.headers["Content-Type"]

    // env["SCRIPT_NAME"] = route.path
    // env["PATH_INFO"] = extract_path_info(request.path, route)

    // env["SERVER_PROTOCOL"] = "HTTP/1.1"
    // env["GATEWAY_INTERFACE"] = "CGI/1.1"

    return map;
}

char**			WebServer::convertMapToChar(const std::map<std::string, std::string>& map){
    (void)map;
    char** tab = NULL;
    // map ti char fonction a faire avec un super malloc
    //attention aux unfructuousmallocs 
    return tab;
}
