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

bool WebServer::isCGI(const LocationConfig* location, const std::string& path){
    //coder un truc

    //aller chercher dans le parsinf du fichier de conf de la loc si on a des formats correspondant
    // depend soit d une extension soit d un flag en config. Faut il y avoir les 2 ou tout fonctionne ? 
    return true;
}

HttpResponse	WebServer::executeCGI(const SocketClient* client, const LocationConfig* location, const std::string& path) {
    //1) faire des verif de chmod et existe
        // if (access(path.c_str(), F_OK) == -1)
            // return buildResponseError(404);
        // if (access(path.c_str(), X_OK) == -1)
            // return buildResponseError(403);
    
    //2) creation du ENVP 
        //creation du envp
        //convert to char**
    
    
    //3) faire des pipes stdin et stdout 
        // int pipe_in[2] = {-1};   // parent → child
        // int pipe_out[2] = {-1};  // child → parent
        // pipe(fd) -> fd[0] et fd[1]
        // if pipe == -1 -> return code erreur 500
        
    //4) faire des fork()
        //pid_t pid = fork()

        //if (pid == -1)
        // safeClose(pipe_in[0]); safeClose(pipe_in[1]);
        // safeClose(pipe_out[0]); safeClose(pipe_out[1]);
        // freeEnvp(&(*envp));
        // return buildErrorResponse(500, client);

        //if (pid == 0)
            // dup2(pipe_in[0], STDIN_FILENO);
            // dup2(pipe_out[1], STDOUT_FILENO);
            // safeClose(pipe_in[0]); safeClose(pipe_in[1]);
            // safeClose(pipe_out[0]); safeClose(pipe_out[1]);

            // char* argv[] = { const_cast<char*>(path.c_str()), NULL }; creation d un tableau *[] pour mettre en tab[0] le paht et tab[1] NULL
            // execve(path.c_str(), argv, envp); 
            
            // gestion de sortie si fail -> quel code d erreur ? 500 ? -> exit(1);
        
        //else

            // close(pipe_in[0]); close(pipe_out[1]);
            
            // recuperation des infos du child
                // write(fd[1], body.c_str(), body.size()); -> est ce que on met dans le ficheir body plus qu une string
                //close(pipe_in[1]);
            
           
            
            // wait ? creation d un timeout ? -> si timeout quelle erreur ?
                // int status;
                // int timeout = 0;
                // const int MAX_WAIT = 5;

                //if (timeout) {
                //     kill(pid, SIGKILL);
                //     return 504; 
                // }

            
            // return crea_d_une_response_special_cgi(), free le tab?

    HttpResponse res;
    return res;
}



HttpResponse	WebServer::executeStatic(const SocketClient* client, const LocationConfig* location, const std::string& path) {
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

char**			WebServer::convertMapToChar(const std::map<std::string, std::string>&){
    char** tab;
    // map ti char fonction a faire avec un super malloc
    //attention aux unfructuousmallocs 
    return tab;
}