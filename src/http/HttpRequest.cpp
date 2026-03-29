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

#include "../http/HttpRequest.hpp"
#include "../utils/utilsParsing.hpp"

/* ************************************************** */
/* construtor & destructors                           */
/* ************************************************** */

HttpRequest::HttpRequest() : _contentLength(0) {}
HttpRequest::~HttpRequest(){}

/* ************************************************** */
/* getters & setters.                                 */
/* ************************************************** */

const std::string& HttpRequest::getMethod() const { return (_method); }
const std::string& HttpRequest::getUri() const { return (_uri); }
const std::string& HttpRequest::getVersion() const { return (_version); }
const std::string& HttpRequest::getBody() const { return(_body); }
long        HttpRequest::getContentLength() const { return(_contentLength); }
const std::map<std::string, std::string>& HttpRequest::getHeaders() const { return (_headers); }

void HttpRequest::setMethod(std::string str){ _method = str; }
void HttpRequest::setUri(std::string str){ _uri = str; }
void HttpRequest::setVersion(std::string str){ _version = str; }
void HttpRequest::setBody(std::string str){ _body = str; }
void HttpRequest::setContentLength(long length) { _contentLength = length; }
void HttpRequest::setHeaders(const std::string& key, const std::string& value){
    _headers[toLower(key)] = value;
}

void HttpRequest::addBody(const std::string& str){
    _body += str;
}
