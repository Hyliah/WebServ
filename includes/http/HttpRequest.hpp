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

#include <iostream>
#include <map>

class HttpRequest
{
    private :
    // REQUEST LINE
    std::string _method; // POST DELETE GET
    std::string _uri; // la target en gros
    std::string _version; // version du http

    //HEADERS
    long    _contentLength;
    std::map<std::string, std::string> _headers; //key : value

    //BODY
    std::string _body; // POST PUT et parfois DELETE -> GET PAS DE BODY /!/

    public :

    HttpRequest();
    HttpRequest(const HttpRequest &other) = delete;
    HttpRequest& operator=(const HttpRequest &other) = delete;
    ~HttpRequest();

    const std::string& getMethod() const;
    const std::string& getUri() const;
    const std::string& getVersion() const;
    const std::string& getBody() const;
    long        getContentLength() const;
    const std::map<std::string, std::string>& getHeaders() const;

    void setMethod(std::string);
    void setUri(std::string);
    void setVersion(std::string);
    void setBody(std::string);
    void setContentLength(long length);
    void setHeaders(const std::string& key, const std::string& value);
};

// METHOD space URI space VERSION


/*
Query string

GET /search?q=chatgpt&page=1 HTTP/1.1

URI :
/search?q=chatgpt&page=1

Tu dois séparer :

path = /search
query = q=chatgpt&page=1



URL encoding
/file%20name.txt

→ %20 = espace




dans les headers, que faire si y en a 2 les memes ?

Erreur 400 bad request ! faire une fonction
singleton : content length, host, content type, authorization, Transfer-Encoding:

*/