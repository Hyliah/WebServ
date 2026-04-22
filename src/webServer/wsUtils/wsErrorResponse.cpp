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


HttpResponse WebServer::buildErrorResponse(int code, const SocketClient* client)
{
    HttpResponse res;

    std::string statusText;
    std::string imagePath;
    std::string message;

    if (code == 400) {
        statusText = "Bad Request";
        imagePath = "/Assets/400.jpg";
        message = "Bad request 🤨";
    }
    else if (code == 403) {
        statusText = "Forbidden";
        imagePath = "/Assets/403.jpg";
        message = "Access denied 🚫";
    }
    else if (code == 404) {
        statusText = "Not Found";
        imagePath = "/Assets/404.jpg";
        message = "Oups... page missing 🦖";
    }
    else if (code == 413) {
        statusText = "Payload Too Large";
        imagePath = "/Assets/413.jpg";
        message = "Body too big 📦";
    }
    else if (code == 414) {
        statusText = "URI Too Long";
        imagePath = "/Assets/414.jpg";
        message = "URL exploded 💥";
    }
    else if (code == 415) {
        statusText = "Unsupported Media Type";
        imagePath = "/Assets/415.jpg";
        message = "Format not supported 📁";
    }
    else if (code == 418) {
        statusText = "I'm a teapot";
        imagePath = "/Assets/418.jpg";
        message = "Tea time ☕";
    }
    else if (code == 431) {
        statusText = "Request Header Fields Too Large";
        imagePath = "/Assets/431.jpg";
        message = "Headers too big 📜";
    }
    else if (code == 500) {
        statusText = "Internal Server Error";
        imagePath = "/Assets/500.jpg";
        message = "Something broke 💀";
    }
    else {
        statusText = "Error";
        imagePath = "/Assets/error.jpg";
        message = "Unknown error";
    }

    // Status line HTTP
    res.statusLine = "HTTP/1.1 " + longToString(code) + " " + statusText;

    // Body HTML
    res.body =
        "<html>"
        "<head><title>" + longToString(code) + " " + statusText + "</title></head>"
        "<body style='text-align:center;font-family:sans-serif;'>"
        "<h1>" + longToString(code) + " - " + statusText + "</h1>"
        "<p>" + message + "</p>"
        "<img src='" + imagePath + "' width='400'>"
        "</body>"
        "</html>";

    // Headers
    res.headers["Content-Length"].push_back(longToString(res.body.size()));
    res.headers["Content-Type"].push_back("text/html");

    if (client && client->keepAlive)
        res.headers["Connection"].push_back("keep-alive");
    else
        res.headers["Connection"].push_back("close");

    return res;
}

// HttpResponse WebServer::buildErrorResponse(int code, const SocketClient* client) {
//     HttpResponse res;

//     std::string statusText = "Error";
// 	std::string	imagePath = "Error";
// 	std::string comment = "";

// 	if (code == 400){
// 		statusText = "Bad Request";
// 		//imagePath = "./doss";
// 		comment = "bibibibb 400";
// 	}
// 	else if (code == 403){
// 		statusText = "Forbidden";
// 		//imagePath = "./doss";
// 		comment = "FOR BI DEN bibibibibbi";
// 	}
//     else if (code == 404){
//         statusText = "Not Found";
// 		imagePath = "./Assets/404.jpg";
// 		comment = "Oups... Prout Dino ate your page 🦖";
// 	}
// 	else if (code == 413){
//         statusText = "Payload too large";
// 		//imagePath = "./doss";
// 		comment = "413 bibibbibibi ";
// 	}
// 	else if (code == 414){
//         statusText = "URI too long";
// 		//imagePath = "./doss";
// 		comment = "414 bibibibiibibibbi ";
// 	}
// 	else if (code == 415){
//         statusText = "Unsupported media type";
// 		//imagePath = "./doss";
// 		comment = "415 bibibibiibibibbi ";
// 	}
// 	else if (code == 418){
//         statusText = "I am a teapot";
// 		//imagePath = "./doss";
// 		comment = "The Server refuses to brew Coffee because it is permanently a TeaPot";
// 	}
// 		else if (code == 431){
//         statusText = "Request Header fields too large";
// 		//imagePath = "./doss";
// 		comment = "431 bibibibiibibibbi ";
// 	}
//     else if (code == 500){
// 		statusText = "Internal Server Error";
// 		//imagePath = "./doss";
// 	}
    
// 	// Status line
//     res.statusLine = "HTTP/1.1 " + longToString(code) + " " + statusText;

//     // Body
//     res.body =
//         "<html>"
//         "<head><title>" + longToString(code) + " " + statusText + "</title></head>"
//         "<body style='text-align:center;'>"
//         "<h1>" + longToString(code) + " - " + statusText + "</h1>"
//         "<p>" + comment + "</p>"
//         //"<img src='" + imagePath + "' width='400'>"
//         "</body>"
//         "</html>";

//     // Headers
// 	std::string sizeStr = longToString(res.body.size());
//     res.headers["Content-Length"].push_back(sizeStr);
//     res.headers["Content-Type"].push_back("text/html");
// 	if (client->keepAlive)
//         res.headers["Connection"].push_back("keep-alive");
// 	else 
//     	res.headers["Connection"].push_back("close");

//     return res;
// }