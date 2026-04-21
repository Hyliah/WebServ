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
#include <cctype>
#include <cstring>
#include <map>

#include <sstream>
#include <ctime>
#include <unistd.h>

//#include "../http/HttpRequest.hpp"
#include "WebServer.hpp"
#include "Exceptions.hpp"


/* ************************************************** */
/* construtor & destructors                           */
/* ************************************************** */

HttpRequest::HttpRequest() : _contentLength(0) {}
HttpRequest::~HttpRequest(){}

/* ************************************************** */
/* getters & setters.                                 */
/* ************************************************** */

const	std::string& HttpRequest::getMethod() const { return (_method); }
const	std::string& HttpRequest::getUri() const { return (_uri); }
const	std::string& HttpRequest::getVersion() const { return (_version); }
const   std::string& HttpRequest::getBodyPath() const { return(_bodyFilePath); }
const   std::string& HttpRequest::getPath() const { return(_path);}
long    HttpRequest::getContentLength() const { return(_contentLength); }
const	std::map<std::string, std::string>& HttpRequest::getHeaders() const { return (_headers); }
const   std::string& HttpRequest::getOriginPath() const{ return(_originPath); }
const   std::string& HttpRequest::getOriginQuery() const{ return(_originQuery); }

void    HttpRequest::setOriginPath(std::string str){ _originPath = str; }
void    HttpRequest::setOriginQuery(std::string str){ _originQuery = str; }
void	HttpRequest::setMethod(std::string str){ _method = str; }
void	HttpRequest::setUri(std::string str){ _uri = str; }
void	HttpRequest::setVersion(std::string str){ _version = str; }
void	HttpRequest::setBodyPath(std::string str){ _bodyFilePath = str; }
void	HttpRequest::setPath(std::string str){ _path = str; }
void	HttpRequest::setContentLength(long length) { _contentLength = length; }
void	HttpRequest::setHeaders(const std::string& key, const std::string& value){
	_headers[toLower(key)] = value;
}
void    HttpRequest::setQuery(std::map<std::string, std::string> query){ _query = query; }

void	HttpRequest::writeBody(const std::string& str){
	if (_bodyFile.is_open())
		_bodyFile.write(str.c_str(), str.size());

}

void	HttpRequest::closeBodyFile() {
	if (_bodyFile.is_open())
		_bodyFile.close();
}

std::string generateId() {
	std::stringstream ss;
	ss << getpid() << "_" << std::time(NULL);
	return ss.str();
}

void	HttpRequest::openBodyFile() {
	_bodyFilePath = "/tmp/webserv_body_" + generateId();
	_bodyFile.open(_bodyFilePath.c_str(), std::ios::binary);
	if (!_bodyFile.is_open()) {
		throw std::runtime_error("Failed to open body file"); // en attendant est ce que il faut envoyer un error 500
	}
}
