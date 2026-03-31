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

SocketClient::SocketClient() : _fd(-1), _bytesRead(0), _bytesPending(0), _buffer(""), _chunkState(CHUNK_SIZE), ignoreBody(false), headerParsed(false), requestCompleted(false), contentLength(false), chunked(false) {}
SocketClient::SocketClient(int fd, struct sockaddr_storage addr) : _fd(fd), _bytesRead(0), _bytesPending(0), _buffer(""), _chunkState(CHUNK_SIZE), _addr(addr), ignoreBody(false), headerParsed(false), requestCompleted(false), contentLength(false), chunked(false) {}
SocketClient::~SocketClient(){}

/* ************************************************** */
/* getters & setters.                                 */
/* ************************************************** */

int					SocketClient::getFd() { return (_fd); }
HttpRequest&		SocketClient::getRequest(){ return (_request); }
const std::string&	SocketClient::getBuffer() const{ return (_buffer); }
long				SocketClient::getBytes(){ return (_bytesRead); }

/* ************************************************** */
/* parsing de la request du Paul                      */
/* ************************************************** */

void	SocketClient::addBytes(long bytes){ _bytesRead += bytes; }
void	SocketClient::appendBuffer(const std::string& str){ _buffer += str; }

void	SocketClient::parseRequest(){

	std::cout << "--- DEBUG BUFFER START ---" << std::endl;
    std::cout << _buffer.substr(0, 100) << "..." << std::endl; // Affiche les 100 premiers caractères
    std::cout << "--- DEBUG BUFFER END ---" << std::endl;

	//size_t header_end = _buffer.find("\r\n\r\n");
	size_t position = 0;
	parseFirstLine(_buffer, position);
	parseHeaders(_buffer, position);

	std::cout << "--- DEBUG BUFFER START AFTER PARSING ---" << std::endl;
    std::cout << _buffer.substr(0, 100) << "..." << std::endl; // Affiche les 100 premiers caractères
    std::cout << "--- DEBUG BUFFER END AFTER PARSING ---" << std::endl;
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
			;
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

void	SocketClient::parseFirstLine(std::string &buffer, size_t &position) {
	size_t line_end = buffer.find("\r\n", position);
	std::string firstLine = buffer.substr(position, line_end - position);

	if (!firstLine.empty() && firstLine.back() == '\r')
		firstLine.pop_back();

	size_t start;
	size_t pos;

	start = 0;
	pos = firstLine.find(' ', start);
	_request.setMethod(firstLine.substr(start, pos - start));
	if (!isValidMethod())
		return ; // error 400 bad request I suppose

	start = pos + 1;
	pos = firstLine.find(' ', start);
	_request.setUri(firstLine.substr(start, pos - start));
	if (!isValidURI())
		return ; // error 400 bad request I suppose

	start = pos + 1;
	_request.setVersion(firstLine.substr(start));
	if (!isValidVersion())
		return ; // error 400 bad request I suppose

	position = line_end + 2;
}

void	SocketClient::parseHeaders(std::string &buffer, size_t &position) {
	size_t header_end = buffer.find("\r\n\r\n");
	size_t start = position;

	while (start < header_end) {
		size_t line_end = buffer.find("\r\n", start);
		if (line_end == std::string::npos) break;
		// check header line size

		std::string line = buffer.substr(start, line_end - start);

		size_t colon = line.find(':');
		if (colon != std::string::npos) {
			std::string key = line.substr(0, colon);
			std::string value = line.substr(colon + 2);

			_request.setHeaders(key, value);
		}
		start = line_end + 2; // passer à la ligne suivante
		
		//check max header size
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

			//on peut faire un if == -1 au pire
			try {
				_bytesPending = hexToLong(line);
			} catch (...) {
				_chunkState = CHUNK_ERROR;
				return;
			}
			_buffer.erase(0, pos + 2); // remove "size\r\n"
			if (_bytesPending == 0) {
				_chunkState = CHUNK_DONE;
				return;
			}

			_chunkState = CHUNK_DATA;
		}

		else if (_chunkState == CHUNK_DATA) {
			if ((long)_buffer.size() < _bytesPending)
				return; // attendre recv
			std::string chunk = _buffer.substr(0, _bytesPending);
			_request.addBody(chunk);
			_bytesRead += _bytesPending;
			if (_bytesRead > DEFAULT_MAX_BODY_SIZE)
				; // erreur dépassement -> HTTP 413 Payload Too Large

			_buffer.erase(0, _bytesPending);

			_chunkState = CHUNK_CRLF;
		}

		else if (_chunkState == CHUNK_CRLF) {
			if (_buffer.size() < 2)
				return;
			if (_buffer.substr(0, 2) != "\r\n") {
				_chunkState = CHUNK_ERROR;
				return;
			}
			_buffer.erase(0, 2);

			_chunkState = CHUNK_SIZE;
		}

		else if (_chunkState == CHUNK_DONE){
			requestCompleted = true;

		}
		
		else { //CHUNK ERROR
			return; //a voir :) 
		}
	}
}

void	SocketClient::parsingContentLength() {
    size_t size = _buffer.size();
    size_t remaining = _request.getContentLength() - _bytesRead;

    if (size > remaining)
        size = remaining;

    if (size > 0) {
        _request.addBody(_buffer.substr(0, size));
        _bytesRead += size;
        _buffer.erase(0, size);
    }

    if (_bytesRead == _request.getContentLength())
        requestCompleted = true;
}

/* ************************************************** */
/* checks Parsing                                     */
/* ************************************************** */

bool	SocketClient::isValidURI(){
	return true;
	//qu est ce qui définit un URI valide ? We ll never know
}

bool	SocketClient::isValidMethod(){
	if (_request.getMethod() == "POST" || _request.getMethod() == "GET" || _request.getMethod() == "DELETE")
		return true;
	return false;
}

bool	SocketClient::isValidVersion(){
	//definir les version qui sont présente. Si 1.1 on sait que c est obligatoire d avoir un host. 
	// faire cette verif une fois que le header est fini de parser.
	//definir le format acceptable d envoi de version.
	return true;
}

bool	SocketClient::isValidBody(){

	const std::string& body = _request.getBody();


	for (size_t i = 0; i < body.size(); ++i) {
        if (!isprint(body[i]) && body[i] != '\n' && body[i] != '\r')
            return false;
    }
	
	if (body.empty() && !ignoreBody) //faire la verif des ignore body 
		return false;

	// Vérifier content-type / encoding
	// Attention aux injections si tu passes le body à un parseur ou script
	// Timeout / limite mémoire si traitement lourd

	return true;
}

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