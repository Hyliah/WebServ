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
#include <fstream>
#include <map>

class HttpRequest
{
    private :
    // REQUEST LINE
    std::string _method; // POST DELETE GET
    std::string _uri; // la target en gros
    std::string _version; // version du http

    std::string _path;
    std::map<std::string, std::string> _query;

    //HEADERS
    long    _contentLength;
    std::map<std::string, std::string> _headers; //key : value
    
    //BODY 
    std::string _bodyFilePath;
    std::ofstream _bodyFile;
    
    HttpRequest(const HttpRequest &other);
    HttpRequest& operator=(const HttpRequest &other);
    
    public :

    HttpRequest();
    ~HttpRequest();

    const   std::string& getMethod() const;
    const   std::string& getUri() const;
    const   std::string& getVersion() const;
    const   std::string& getBodyPath() const;
    const   std::string& getPath() const;
    const   std::map<std::string, std::string>& getHeaders() const;
    long    getContentLength() const;

    void    setMethod(std::string);
    void    setUri(std::string);
    void    setVersion(std::string);
    void    setBodyPath(std::string);
    void    setPath(std::string);
    void    setContentLength(long length);
    void    setHeaders(const std::string& key, const std::string& value);
    void    setQuery(std::map<std::string, std::string> query);


    void    writeBody(const std::string& str);
    void    closeBodyFile();
    void    openBodyFile();
};

// METHOD space URI space VERSION



// Query string

// GET /search?q=chatgpt&page=1 HTTP/1.1

// URI :
// /search?q=chatgpt&page=1

// Tu dois séparer :

// path = /search
// query = q=chatgpt&page=1



// URL encoding
// /file%20name.txt

// → %20 = espace




// dans les headers, que faire si y en a 2 les memes ?

// Erreur 400 bad request ! faire une fonction
// singleton : content length, host, content type, authorization, Transfer-Encoding:

