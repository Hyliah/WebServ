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

#include "WebServer.hpp"

httpResponse WebServer::buildErrorResponse(int code) {
    httpResponse res;

    std::string statusText = "Error";
	//std::string	imagePath = "Error";
	std::string comment = ""

	if (code == 400){
		statusText = "Bad Request";
		//imagePath = "./doss";
		comment = "";
	}
	else if (code == 403){
		statusText = "Forbidden";
		//imagePath = "./doss";
		comment = "";
	}
    else if (code == 404){
        statusText = "Not Found";
		//imagePath = "./doss";
		comment = "Oups... Prout Dino ate your page 🦖";
	}
	else if (code == 413){
        statusText = "Payload too large";
		//imagePath = "./doss";
		comment = "";
	}
	else if (code == 414){
        statusText = "URI too long";
		//imagePath = "./doss";
		comment = "";
	}
	else if (code == 418){
        statusText = "I am a teapot";
		//imagePath = "./doss";
		comment = "The Server refuses to brew Coffee because it is permanently a TeaPot";
	}
    else if (code == 500){
		statusText = "Internal Server Error";
		//imagePath = "./doss";
	}
    
	// Status line
    res.statusLine = "HTTP/1.1 " + longToString(code) + " " + statusText;

    // Body
    res.body =
        "<html>"
        "<head><title>" + longToString(code) + " " + statusText + "</title></head>"
        "<body style='text-align:center;'>"
        "<h1>" + longToString(code) + " - " + statusText + "</h1>"
        "<p>" + comment + "</p>"
        //"<img src='" + imagePath + "' width='400'>"
        "</body>"
        "</html>";

    // Headers
    res.headers["Content-Length"] = longToString(res.body.size());
    res.headers["Content-Type"] = "text/html";
    res.headers["Connection"] = "close";

    return res;
}