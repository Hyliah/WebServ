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
    HttpRequest(const HttpRequest &other); //utile ou = delete ?
    HttpRequest& operator=(const HttpRequest &other);
    ~HttpRequest();

    std::string getMethod() const;
    std::string getUri() const;
    std::string getVersion() const;
    std::string getBody() const;
    std::map<std::string, std::string>& getHeaders();
    void setMethod(std::string);
    void setUri(std::string);
    void setVersion(std::string);
    void setBody(std::string);
    void setHeaders(std::string key, std::string value);
    // _data.insert(std::make_pair(date, value));
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


*/