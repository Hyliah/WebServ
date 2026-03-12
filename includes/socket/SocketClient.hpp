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

class SocketClient {

private:

    int         _fd;
    std::string _buffer;
    int         _port;

public:

    SocketClient();
    SocketClient(int fd);
	SocketClient(const SocketClient& other);
	SocketClient& operator=(const SocketClient& other);
    ~SocketClient();

    int receiveData();
    int sendData(const std::string& data);

    void closeSocket();

    int getFd() const;

    const std::string& getBuffer() const;

    int getPort() const;
    void setPort(int port);
};

#endif