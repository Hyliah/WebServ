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
#include "../utils/utilsParsing.hpp"

/* ************************************************** */
/* construtor & destructors                           */
/* ************************************************** */

SocketClient::SocketClient() : _fd(-1), _buffer(""), headerParsed(false), requestCompleted(false), contentLength(false), chunked(false){}
SocketClient::SocketClient(int fd, struct sockaddr_storage addr) : _fd(fd), _buffer(""), headerParsed(false), contentLength(false), chunked(false), requestCompleted(false), _addr(addr) {}
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

void SocketClient::appendBuffer(const std::string& str){
    _buffer += str; //mettre des verif ?
}

void SocketClient::parseRequest(){
    size_t header_end = _buffer.find("\r\n\r\n");
    
    size_t position = 0;
    parseFirstLine(_buffer, position);
    parseHeaders(_buffer, position);

    //je le laisse au cas ou y a une une erreur il passera pas en true
    headerParsed = true;
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

    // position = header_end + 4; // début du body // Je gere ca plus loin du coup
}

/* ************************************************** */
/* je ferai qu on y sera.                             */
/* ************************************************** */

void SocketClient::closeSocket(){
}

void SocketClient::handleLength(){

    const std::map<std::string, std::string>& headers = _request.getHeaders();
    
    //Content Length
    std::map<std::string, std::string>::const_iterator itCL;
    itCL = headers.find("content-length");
    if (itCL != headers.end()){
        contentLength = true;
        _request.setContentLength(stringToLong(itCL->second.c_str()));
        // attention gerer les 400 ou 413 ou quoi si le string n est pas un noombre correct genre 10M
        // peut etre faire une verif avant pour pas changer la fonction stringToLong
    }
    else {
        contentLength = false;
        _request.setContentLength(0);
    }

    //chunked
    chunked = false;
    std::map<std::string, std::string>::const_iterator itTE;
    itTE = headers.find("transfer-encoding");
    if (itTE != headers.end()){
        std::string value = toLower(itTE->second);
        if (value.find("chunked") != std::string::npos)
            chunked = true;
    }
    if (chunked)
        contentLength = false;     
}






// faut metttre un bool comme quoi on a ou pas un contentlength
// mettre aussi un bool pour le chunk
// ensuite gerer les 3 cas :
// 1. pas de chunk et pas de content-lenth -> on s arrete au header
// 1.5 verifier que les deux trucs de longueur ne soient pas a true tout les deux -> sinon -> passe a chunked
// 3. gerer le chunked -> apprendre a fair ca. 
// 2. content length (without chunked) oui mais pas chunked -> on parse jusqu a la taille definie

// int SocketClient::receiveData(){
//     return 1; //pour qu il ne casse pas les couillasses
// }
// int SocketClient::sendData(const std::string& data){
//     return 1; //pour qu il ne casse pas les couillasses
// }






void SocketClient::parseBody(std::string &buffer, size_t &position) {
    if (position >= buffer.size())
        return;

    // size_t content_length = 0;
    // auto &headers = _request.getHeaders();
    // if (headers.count("Content-Length")) {
    //     content_length = std::stoul(headers["Content-Length"]);                        //??????
    //}

    // attention à ne pas dépasser le buffer
    // size_t available = buffer.size() - position;
    // size_t to_read = std::min(content_length, available);

    _request.setBody(buffer.substr(position, buffer.size()));
}