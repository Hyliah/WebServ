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
#include "../utils/utilsParsing.hpp"
#include "Exceptions.hpp"

/* ************************************************** */
/* construtor & destructors                           */
/* ************************************************** */

SocketClient::SocketClient() : _fd(-1), _bytesRead(0), _bytesPending(0), _buffer(""), _chunkState(CHUNK_SIZE), _server(NULL), ignoreBody(false), headerParsed(false), requestCompleted(false), contentLength(false), chunked(false), keepAlive(true) {
	lastActivity = std::time(NULL);
}
SocketClient::SocketClient(int fd, struct sockaddr_storage addr, SocketServer* serverPtr) : _fd(fd), _bytesRead(0), _bytesPending(0), _buffer(""), _chunkState(CHUNK_SIZE), _server(serverPtr), _addr(addr), ignoreBody(false), headerParsed(false), requestCompleted(false), contentLength(false), chunked(false) {}
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

	LOG(">>> PARSING REQUEST");

	size_t position = 0;
	parseFirstLine(_buffer, position);
		LOG(">>> LINE FIRSAT OKKKKKKKKKK");
	parseHeaders(_buffer, position);
		LOG(">>> HEADERS OOKAKKAKAKAKAKKAKAKA");
}

void	SocketClient::defineBodyType(){

	const std::map<std::string, std::string>& headers = _request.getHeaders();


	//Content Length
	std::map<std::string, std::string>::const_iterator itCL;
	itCL = headers.find("content-length");
	if (itCL != headers.end()){
		contentLength = true;
		// peut etre faire une verif avant pour pas changer la fonction stringToLong
		// attention gerer les 400 ou 413 ou quoi si le string n est pas un noombre correct genre 10M
		_request.setContentLength(stringToLong(itCL->second.c_str()));
		if (_request.getContentLength() > DEFAULT_MAX_BODY_SIZE)
			throw ResponseException(_fd, 413); //CHANGER LE NUMERO QUAND JE LE CONNAITRAI !!!!!!!!!!!!!!!!!!!!!!
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

/* ************************************************** */
/* parsing firstLine & Headers                        */
/* ************************************************** */

void SocketClient::parseFirstLine(std::string &buffer, size_t &position)
{
    size_t line_end = buffer.find("\r\n", position);
    if (line_end == std::string::npos)
        throw ResponseException(_fd, 400);

    std::string firstLine = buffer.substr(position, line_end - position);

    if (!firstLine.empty() && firstLine.back() == '\r')
        firstLine.pop_back();

    size_t start = 0;
    size_t pos = firstLine.find(' ', start);
    if (pos == std::string::npos)
        throw ResponseException(_fd, 400);

    std::string method = firstLine.substr(start, pos - start);
    _request.setMethod(method);
    if (!isValidMethod())
        throw ResponseException(_fd, 400);

    start = pos + 1;
    pos = firstLine.find(' ', start);
    if (pos == std::string::npos)
        throw ResponseException(_fd, 400);

    std::string uri = firstLine.substr(start, pos - start);
    _request.setUri(uri);
    if (!isValidURI())
        throw ResponseException(_fd, 414);

    start = pos + 1;
    std::string version = firstLine.substr(start);
    _request.setVersion(version);
    if (!isValidVersion())
        throw ResponseException(_fd, 400);

    position = line_end + 2;

    LOG("Parsing first line: " << firstLine);
}

void SocketClient::parseHeaders(std::string &buffer, size_t &position) {
    size_t header_end = buffer.find("\r\n\r\n");
	if (header_end == std::string::npos)
    	throw ResponseException(_fd, 400);
	
	size_t start = position;

    int headerCount = 0;
    size_t totalHeaderSize = 0;

    while (start < header_end) {
        size_t line_end = buffer.find("\r\n", start);
        if (line_end == std::string::npos)
            break;

        std::string line = buffer.substr(start, line_end - start);

        if (line.size() > MAX_HEADER_LINE_SIZE)
            throw ResponseException(_fd, 431);

        size_t colon = line.find(':');
        if (colon == std::string::npos ){
            throw ResponseException(_fd, 400);
		}

        std::string key = line.substr(0, colon);
		LOG("------------------------ Key AVANT : " << key);
		key = toLower(key);
		LOG("------------------------ Key APRES : " << key);
        std::string value = line.substr(colon + 2);

        _request.setHeaders(key, value);

        headerCount++;
        totalHeaderSize += line.size();

        start = line_end + 2;
    }

    validateHeaders(headerCount, totalHeaderSize);
}

void SocketClient::validateHeaders(int headerCount, size_t totalSize) { // --------------------------------------------------------------------------------------------

    const std::map<std::string, std::string>& h = _request.getHeaders();

    if (headerCount > MAX_HEADER_COUNT)
        throw ResponseException(_fd, 431);

    if (totalSize > MAX_HEADER_SIZE)
        throw ResponseException(_fd, 431);

    if (h.find("host") == h.end()){
        throw ResponseException(_fd, 400);
	}

    std::map<std::string, std::string>::const_iterator it = h.find("content-length");
    if (it != h.end()) {
        if (it->second.find_first_not_of("0123456789") != std::string::npos)
		{
            throw ResponseException(_fd, 400);
		}
    }
}

/* ************************************************** */
/* Parsing Body                                       */
/* ************************************************** */

void	SocketClient::parsingNoBody(){
	requestCompleted = true;
}

void SocketClient::parsingChunked() {
	while (1) {

		if (_chunkState == CHUNK_SIZE) {

			size_t pos = _buffer.find("\r\n");
			if (pos == std::string::npos)
				return; // attendre recv()
			
			std::string line = _buffer.substr(0, pos);

			try { _bytesPending = hexToLong(line); } 
			catch (...) {
				throw ResponseException(_fd, 400);
			}

			_buffer.erase(0, pos + 2); // remove "size\r\n"

			if (_bytesPending == 0) {
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
				
			_request.writeBody(chunk);

			_bytesRead += _bytesPending;
			_buffer.erase(0, _bytesPending);

			_chunkState = CHUNK_CRLF;
		}

		else if (_chunkState == CHUNK_CRLF) {
			if (_buffer.size() < 2)
				return;
			if (_buffer.substr(0, 2) != "\r\n") {
				throw ResponseException(_fd, 400);
			}
			_buffer.erase(0, 2);

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

        if (!isValidBody(chunk))
            throw ResponseException(_fd, 400);

        _request.writeBody(chunk);

        _bytesRead += size;
        _buffer.erase(0, size);
    }

    if (_bytesRead == _request.getContentLength()) {
        requestCompleted = true;
        _request.closeBodyFile();
    }
}

/* ************************************************** */
/* checks Parsing                                     */
/* ************************************************** */

// verifier si y a un host
bool	SocketClient::isValidURI(){
	 
	return true;
}

bool	SocketClient::isValidMethod(){
	if (_request.getMethod() == "POST" || _request.getMethod() == "GET" || _request.getMethod() == "DELETE")
		return true;
	return false;
}

bool	SocketClient::isValidVersion(){
	//definir 1.1 or nothing bitches
	return true;
}

bool	SocketClient::isValidBody(std::string& chunk){

    const std::map<std::string, std::string>& headers = _request.getHeaders();
    std::map<std::string, std::string>::const_iterator it = headers.find("content-type");

    // pas de content-type → on accepte
    if (it == headers.end())
        return true;

    std::string type = it->second;

    // TEXT ONLY
    if (type.find("text") != std::string::npos ||
        type.find("application/json") != std::string::npos ||
        type.find("application/x-www-form-urlencoded") != std::string::npos) {

        for (size_t i = 0; i < chunk.size(); ++i) {
            unsigned char c = chunk[i];

            if (!isprint(c) && c != '\n' && c != '\r' && c != '\t')
                return false;
        }
    }

    // BINAIRE → on accepte
    return true;
}
	// Vérifier content-type / encoding
	// Attention aux injections si tu passes le body à un parseur ou script
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





// ancien ParseBody 


// void SocketClient::parseBody(std::string &buffer, size_t &position) {
// 	if (position >= buffer.size())
// 		return;

// 	// size_t content_length = 0;
// 	// auto &headers = _request.getHeaders();
// 	// if (headers.count("Content-Length")) {
// 	//     content_length = std::stoul(headers["Content-Length"]);                        //??????
// 	//}

// 	// attention à ne pas dépasser le buffer
// 	// size_t available = buffer.size() - position;
// 	// size_t to_read = std::min(content_length, available);

// 	_request.setBody(buffer.substr(position, buffer.size()));
// }



















//base chunked

// void	SocketClient::parsingChunked(){
// 	if (ignoreBody)
// 		return;

// 	// boucle
// 	// 1ere etape : check size -> lire cmb de bytes arrivent
// 	/*
// 		chercher le rn. tout ce que y a avant -> hex to long -> si pb PROBLEM
// 	*/
// 	// 2e etape : check data -> lire ces bytes + addbytes si on pu lire CONTINUE/BREAK
// 	/*
// 		va chercher le prochain rn -> si y a pas on arrive chunked
// 		si oui :
// 			prend le bout du buffer pour metre dans hhtprequest body (faire des veri si plus long )
// 			addbytes
// 			verif maxbody size 
// 	*/
// 	// 3e etape : CRFL -> verifier le rn =? DONE

// 	while (1){
// 		/*

// 		dans buffer aller jusqu'a \r\n
// 		-> si pas trouvé -> RETURN pcq pas assez d info, on attend le prochain recv()
// 		-> si trouvé 
// 			-> extraction du hexa en str
// 			-> bytesPending = hexToLong + verif (if = -1 == PROBLEM)
// 			-> supprime jusqua \r\n inclus
		
// 		lire jusuq au prochain \r\n
// 		-> si y a plus que bytesPending -> PROBLEM
// 		-> si y a pas de \r\n -> RETURN 
// 		-> si y a et que ca fait la taille fait la meme que bytePending
// 			-> on extrait le mot qu on met dans body de httprequest
// 			-> addBytes(bytes); -> client.bytesread
		
// 		verif de bytesRead pour MAX SIZE
// 		*/
// 	}

// 	/*
// 		check de si c est la fin
// 		-> DONE = DONE
// 	*/

// 		// 4\r\nWiki\r\n
// 		// 5\r\npedia\r\n
// 		// 0\r\n\r\n
	
// 	// apprendre a comprendre comment gerer ca. Si je suis bien on a :

// 	// nombre x + rn + texte(size of x) + rn 
// 	// -> rn = x					-> si pas size en int 			-> PROBLEM 
// 	// -> passer rn					-> si deja a la fin 			-> PROBLEM
// 	// -> buffer += line(size of x) -> si texte plus grand que x 	-> PROBLEM
// 	//							-> si pas rn apres				-> PROBLEM
	
// 	// if DONE 
// 		requestCompleted = true;
// }