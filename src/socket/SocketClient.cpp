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
#include "WebServer.hpp"
#include <sstream>
#include "Exceptions.hpp"

/* ************************************************** */
/* construtor & destructors                           */
/* ************************************************** */

SocketClient::SocketClient() : _fd(-1), _bytesRead(0), _bytesPending(0), _buffer(""), _chunkState(CHUNK_SIZE), _server(NULL), ignoreBody(false), headerParsed(false), requestCompleted(false), contentLength(false), chunked(false), keepAlive(true), errorCode(0), lastActivity(std::time(NULL)), state(READING){
}
SocketClient::SocketClient(int fd, struct sockaddr_storage addr, SocketServer* serverPtr) : _fd(fd), _bytesRead(0), _bytesPending(0), _buffer(""), _chunkState(CHUNK_SIZE), _server(serverPtr), _addr(addr), ignoreBody(false), headerParsed(false), requestCompleted(false), contentLength(false), chunked(false), keepAlive(true), errorCode(0), lastActivity(std::time(NULL)), state(READING){}
SocketClient::~SocketClient(){}

/* ************************************************** */
/* getters & setters.                                 */
/* ************************************************** */

int					SocketClient::getFd() const { return (_fd); }
long				SocketClient::getBytes() { return (_bytesRead); }
const HttpRequest&	SocketClient::getRequest() const { return (_request); }
HttpRequest& 		SocketClient::getRequest() { return _request; }
const std::string&	SocketClient::getBuffer() const{ return (_buffer); }
const SocketServer*	SocketClient::getServer() const { return (_server); }

/* ************************************************** */
/* parsing de la request du Paul                      */
/* ************************************************** */

void	SocketClient::addBytes(long bytes){ _bytesRead += bytes; }
void	SocketClient::appendBuffer(const std::string& str){ _buffer += str; }

void	SocketClient::parseRequest(){
	LOG(">>> PARSING REQUEST"); // ----------------------------------------------------------------------------------

	size_t headerEnd = _buffer.find("\r\n\r\n");
	size_t position = 0;
    
	if (headerEnd == std::string::npos)
        return ;

	parseFirstLine(_buffer, position);
	parseHeaders(_buffer, position);

	_buffer.erase(0, headerEnd + 4);
	LOG("Buffer after header erase: [" << _buffer << "]");

	//LOG(">>> HEADER COMPLETE"); // ---------------------------------------------------------------------------------
	//LOG(".    Method: " << getRequest().getMethod()); // ----------------------------------------------------------------
	//LOG(".    URI: " << getRequest().getUri()); // ----------------------------------------------------------------

	if (_request.getMethod() == "POST")
		defineBodyType();

	headerParsed = true;
    state = (chunked || contentLength) ? BODY_READING : READY;
}


void	SocketClient::defineBodyType(){

	const std::map<std::string, std::vector<std::string> >& headers = _request.getHeaders();

	//Content Length
	std::map<std::string, std::vector<std::string> >::const_iterator itCL;
	itCL = headers.find("content-length");
	if (itCL != headers.end()){
		contentLength = true;

		_request.setContentLength(stringToLong(itCL->second[0].c_str()));
		if (_request.getContentLength() <= 0) // change ici 
			throw ResponseException(_fd, 405);
		if (_request.getContentLength() > DEFAULT_MAX_BODY_SIZE || _request.getContentLength() <= 0) // change ici 
			throw ResponseException(_fd, 413);
		//LOG("..............  is requete ContentLength: " << contentLength); // -------------------------------------------------------------
		//LOG("..............  La length de la requete ContentLength: " << _request.getContentLength()); // ----------------------------------
	}
	else {
		contentLength = false;
		_request.setContentLength(0);
	}

	//chunked
	chunked = false;
	std::map<std::string, std::vector<std::string> >::const_iterator itTE;
	itTE = headers.find("transfer-encoding");
	if (itTE != headers.end()){
		std::string value = toLower(itTE->second[0]);
		if (value.find("chunked") != std::string::npos)
			chunked = true;
	}

	LOG("..............  is requete Chuncked: " << chunked); // -------------------------------------------------------------

	if(!contentLength && !chunked)
		throw ResponseException(_fd, 405);

	if (chunked)
		contentLength = false;

	if (chunked || contentLength){
		_request.openBodyFile(_fd);
		LOG("OPEN BODY FILE for fd = " << _fd);
		LOG("BODY FILE = " << _request.getBodyPath() << " fd=" << _fd);
	}
}

/* ************************************************** */
/* parsing firstLine & Headers                        */
/* ************************************************** */

void SocketClient::parseFirstLine(std::string &buffer, size_t &pos)
{
    size_t end = buffer.find("\r\n", pos);
    if (end == std::string::npos)
        throw ResponseException(_fd, 400);

    std::string line = buffer.substr(pos, end - pos);

    size_t s1 = line.find(' ');
    if (s1 == std::string::npos)
        throw ResponseException(_fd, 400);

    size_t s2 = line.find(' ', s1 + 1);
    if (s2 == std::string::npos)
        throw ResponseException(_fd, 400);

    std::string method = line.substr(0, s1);
    std::string uri = line.substr(s1 + 1, s2 - s1 - 1);
	if (uri.size() > MAX_URI_SIZE)
    	throw ResponseException(_fd, 414);
    std::string version = line.substr(s2 + 1);

	_request.setMethod(method);
	_request.setUri(uri);
	_request.setVersion(version);

    if (!isValidMethod())
        throw ResponseException(_fd, 405);

    if (!isValidURI())
        throw ResponseException(_fd, 400);

    if (!isValidVersion())
        throw ResponseException(_fd, 400);


    pos = end + 2;
}

void SocketClient::parseHeaders(std::string &buffer, size_t &pos)
{
    size_t end = buffer.find("\r\n\r\n", pos);
    if (end == std::string::npos)
        throw ResponseException(_fd, 400);

    int count = 0;
    size_t total = 0;

    while (pos < end)
    {
        size_t lineEnd = buffer.find("\r\n", pos);
        if (lineEnd == std::string::npos || lineEnd > end)
            break;

        std::string line = buffer.substr(pos, lineEnd - pos);

        if (line.empty()) {
            pos = lineEnd + 2;
            continue;
        }

        size_t colon = line.find(':');
        if (colon == std::string::npos)
            throw ResponseException(_fd, 400);

        std::string key = trim(line.substr(0, colon));
        std::string value = trim(line.substr(colon + 1));

        if (key.empty())
            throw ResponseException(_fd, 400);

        _request.setHeaders(toLower(key), value);

        count++;
        total += line.size();

        pos = lineEnd + 2 ; // +2 a la base
		//pos = end + 2; // +2 a la base
    }

    validateHeaders(count, total);

    state = HEADERS_PARSED;
}



/* ************************************************** */
/* Parsing Body                                       */
/* ************************************************** */

void	SocketClient::parsingNoBody(){
	requestCompleted = true;
}

void SocketClient::parsingChunked() {
	while (1) {
		//LOG("chunked est passé 1x de plus"); // ----------------------------------------------------------------------------
		if (_chunkState == CHUNK_SIZE) {

			size_t pos = _buffer.find("\r\n");
			if (pos == std::string::npos)
				return; // attendre recv()
			
			std::string line = _buffer.substr(0, pos);
			//LOG("LINE en cours =  " << line); // ----------------------------------------------------------------------------

			try { _bytesPending = hexToLong(line); } 
			catch (...) {
				throw ResponseException(_fd, 400);
			}

			_buffer.erase(0, pos + 2); // remove "size\r\n"

			if (_bytesPending == 0) {
				//LOG("CHUNKED DONE"); // ----------------------------------------------------------------------------
				_chunkState = CHUNK_DONE;
				continue;
			}

			_chunkState = CHUNK_DATA;
		}

		else if (_chunkState == CHUNK_DATA) {
			if ((long)_buffer.size() < _bytesPending)
				return; // attendre recv

			std::string chunk = _buffer.substr(0, _bytesPending);

			if (!isValidBody(chunk))
				throw ResponseException(_fd, 400);
			
			if (_bytesRead + _bytesPending > DEFAULT_MAX_BODY_SIZE)
				throw ResponseException(_fd, 413);
				
			_request.writeBody(chunk, _fd);
			LOG("WRITE BODY fd=" << _fd << " size=" << chunk.size());

			_bytesRead += _bytesPending;
			_buffer.erase(0, _bytesPending);
			
			LOG("CHUNKED CRLF"); // ----------------------------------------------------------------------------
			_chunkState = CHUNK_CRLF;
		}

		else if (_chunkState == CHUNK_CRLF) {
			if (_buffer.size() < 2)
				return;
			if (_buffer.substr(0, 2) != "\r\n") {
				throw ResponseException(_fd, 400);
			}
			_buffer.erase(0, 2);

			LOG("CHUNKED SIZE"); // ----------------------------------------------------------------------------
			_chunkState = CHUNK_SIZE;
		}

		else if (_chunkState == CHUNK_DONE){
			requestCompleted = true;
			return;
		}
		
		else { //CHUNK ERROR
			throw ResponseException(_fd, 418); //a voir quel NB LUI DONNER une error dans le body 400 sinon
		}
	}
}

void SocketClient::parsingContentLength() {
    size_t size = _buffer.size();
    size_t remaining = _request.getContentLength() - _bytesRead;

    if (size > remaining)
        size = remaining;

    if (size > 0) {
        std::string chunk = _buffer.substr(0, size);

		//CHECK MAX SIZE 

        if (!isValidBody(chunk)){
			//LOG("BETTA WORK BITCH");
            throw ResponseException(_fd, 400);
		}

        _request.writeBody(chunk, _fd);
		LOG("WRITE BODY fd=" << _fd << " size=" << chunk.size());

        _bytesRead += size;
        _buffer.erase(0, size);
    }

    if (_bytesRead == _request.getContentLength()) {
        requestCompleted = true;
        _request.closeBodyFile();
    }

	// LOG("DEBUG CL:");
	// LOG("buffer size = " << _buffer.size());
	// LOG("remaining = " << remaining);
	// LOG("bytesRead = " << _bytesRead);
	// LOG("contentLength = " << _request.getContentLength());
	// LOG("writing size = " << size);
}

/* ************************************************** */
/* checks Parsing                                     */
/* ************************************************** */

void SocketClient::validateHeaders(int headerCount, size_t totalSize) {

    const std::map<std::string, std::vector<std::string> >& header = _request.getHeaders();

    if (headerCount > MAX_HEADER_COUNT)
        throw ResponseException(_fd, 431);

    if (totalSize > MAX_HEADER_SIZE)
        throw ResponseException(_fd, 431);

    if (header.find("host") == header.end()){
		LOG("DONT YOU KNOW THAT YOU RE TOXIC");
        throw ResponseException(_fd, 400);
	}

	//Faire une verif de si y a plusieurs host dans le les headers et d autres ?

    std::map<std::string, std::vector<std::string> >::const_iterator it = header.find("content-length");
    if (it != header.end()) {
        if (it->second[0].find_first_not_of("0123456789") != std::string::npos)
		{
			LOG("BREAK THE ICE");
            throw ResponseException(_fd, 400);
		}
    }
}

bool	SocketClient::isValidURI(){

	if (_request.getUri().find("..") != std::string::npos)
		return false;
	return true;
}

bool	SocketClient::isValidMethod(){
	if (_request.getMethod() == "POST" || _request.getMethod() == "GET" || _request.getMethod() == "DELETE")
		return true;
	return false;
}

bool	SocketClient::isValidVersion(){
	if (_request.getVersion() != "HTTP/1.1")
		return false;
	return true;
}

bool	SocketClient::isValidBody(std::string& chunk){
	(void)chunk;
    // const std::map<std::string, std::vector<std::string> >& headers = _request.getHeaders();
    // std::map<std::string, std::vector<std::string> >::const_iterator it = headers.find("content-type");

    // // pas de content-type → on accepte
    // if (it == headers.end())
    //     return true;

    // std::string type = it->second[0];

    // // TEXT ONLY
    // if (type.find("text") != std::string::npos ||
    //     type.find("application/json") != std::string::npos ||
    //     type.find("application/x-www-form-urlencoded") != std::string::npos) {

    //     for (size_t i = 0; i < chunk.size(); ++i) {
    //         unsigned char c = chunk[i];

    //         if (!isprint(c) && c != '\n' && c != '\r' && c != '\t')
    //             return false;
    //     }
    // }

    return true;
}
	// Vérifier content-type / encoding
	// Attention aux injections le body à un parseur ou script
	// Timeout / limite mémoire si traitement lourd - 413 payload too large

bool	SocketClient::isDone() const {
	return (_chunkState == CHUNK_DONE);
}

void	SocketClient::cleanBuffer(){
	size_t pos = _buffer.find("\r\n\r\n");

	_buffer.erase(0, pos + 4);
}

/* ************************************************** */
/* je ferai qu on y sera.                             */
/* ************************************************** */

void	SocketClient::closeSocket(){
}
