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
#include <sstream>
#include <map>

class HttpResponse {
	private:
		// HttpResponse(const HttpResponse& other);
		// HttpResponse& operator=(const HttpResponse& other);
	
	public:
		std::string statusLine;
		std::map<std::string, std::vector<std::string> > headers;
		std::string body;

		std::string ResponseToString() const;

		HttpResponse();
		~HttpResponse();

};