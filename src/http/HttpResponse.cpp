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

#include "HttpResponse.hpp"

std::string HttpResponse::ResponseToString() const {
	std::stringstream ss;
	ss << statusLine << "\r\n";
	for (std::map<std::string,std::string>::const_iterator it = headers.begin();
			it != headers.end(); ++it) {
		ss << it->first << ": " << it->second << "\r\n";
	}
	ss << "\r\n" << body;
	return ss.str();
}
