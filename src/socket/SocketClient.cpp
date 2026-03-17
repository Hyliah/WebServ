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

#include "SocketClient.hpp"
#include <sstream>

/* ************************************************** */
/* construtor & destructors                           */
/* ************************************************** */

SocketClient::SocketClient() : _fd(-1), _buffer(""), _headerParsed(false), _requestCompleted(false) {}
SocketClient::SocketClient(int fd) : _fd(fd), _buffer(""), _headerParsed(false), _requestCompleted(false) {}
SocketClient::~SocketClient(){}

/* ************************************************** */
/* getters & setters.                                 */
/* ************************************************** */


int SocketClient::getFd() const{ return (_fd); }
HttpRequest& SocketClient::getRequest(){ return (_request); }
const std::string& SocketClient::getBuffer() const{ return (_buffer); }

/* ************************************************** */
/* parsing de la request du Paul                      */
/* ************************************************** */

void SocketClient::appendBuffer(std::string str){
    _buffer += str; //mettre des verif ?
}

bool SocketClient::parseRequest(){
    // faire la gestion les Pauls et maintenant on part du principe que le Buffer a fini d etre rempli
    size_t header_end = _buffer.find("\r\n\r\n");
    if (header_end == std::string::npos)
        return false;
    
    size_t position = 0;
    parseFirstLine(_buffer, position);
    parseHeaders(_buffer, position);
    parseBody(_buffer, position);

    _requestCompleted = true;
    return true;
}
    
void SocketClient::parseFirstLine(std::string &buffer, size_t &position) {
    size_t line_end = buffer.find("\r\n", position);
    std::string firstLine = buffer.substr(position, line_end - position);

    if (!firstLine.empty() && firstLine.back() == '\r')
        firstLine.pop_back();

    size_t start;
    size_t pos;

    start = 0;
    pos = firstLine.find(' ', start);
    _request.setMethod(firstLine.substr(start, pos - start));

    start = pos + 1;
    pos = firstLine.find(' ', start);
    _request.setUri(firstLine.substr(start, pos - start));

    start = pos + 1;
    _request.setVersion(firstLine.substr(start));

    position = line_end + 2;
}

void SocketClient::parseHeaders(std::string &buffer, size_t &position) {
    size_t header_end = buffer.find("\r\n\r\n");
    size_t start = position;

    while (start < header_end) {
        size_t line_end = buffer.find("\r\n", start);
        if (line_end == std::string::npos) break;

        std::string line = buffer.substr(start, line_end - start);

        size_t colon = line.find(':');
        if (colon != std::string::npos) {
            std::string key = line.substr(0, colon);
            std::string value = line.substr(colon + 2);

            _request.setHeaders(key, value);
        }
        start = line_end + 2; // passer à la ligne suivante
    }

    position = header_end + 4; // début du body
}

void SocketClient::parseBody(std::string &buffer, size_t &position) {
    if (position >= buffer.size())
        return;

    size_t content_length = 0;
    auto &headers = _request.getHeaders();
    if (headers.count("Content-Length")) {
        content_length = std::stoul(headers["Content-Length"]);
    }

    // attention à ne pas dépasser le buffer
    size_t available = buffer.size() - position;
    size_t to_read = std::min(content_length, available);

    _request.setBody(buffer.substr(position, to_read));
}

// \r\n fait partie du protocole HTTP peu importe l os utilisé 

/* ************************************************** */
/* je ferai qu on y sera.                             */
/* ************************************************** */

void SocketClient::closeSocket(){
}





// int SocketClient::receiveData(){
//     return 1; //pour qu il ne casse pas les couillasses
// }
// int SocketClient::sendData(const std::string& data){
//     return 1; //pour qu il ne casse pas les couillasses
// }