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

#ifndef SOCKETCLIENT_HPP
#define SOCKETCLIENT_HPP

#include <string>
#include <unistd.h>

#include "HttpRequest.hpp"

class SocketClient {

private:

    int         _fd;
    std::string _buffer; //recupéré avec recv() - attention en plusieurs fois

    HttpRequest _request;

    bool        _headerParsed;
    bool        _requestCompleted;

    
    public:
    
    SocketClient();
    SocketClient(int fd);
	SocketClient(const SocketClient& other) = delete;
	SocketClient& operator=(const SocketClient& other) = delete;
    ~SocketClient();
    
    
    void closeSocket();
    
    int getFd() const;
    HttpRequest& getRequest();
    
    const std::string& getBuffer() const;
    
    void appendBuffer(std::string);
    
    bool parseRequest();
    void parseFirstLine(std::string &buffer, size_t &position);
    void parseHeaders(std::string &buffer, size_t &position);
    void parseBody(std::string &buffer, size_t &position);

    // int receiveData();
    // int sendData(const std::string& data);

};

#endif