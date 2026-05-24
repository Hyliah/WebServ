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
    std::string connection;


    if (code == 400) {
        statusText = "Bad Request";
        imagePath = "/Assets/400.jpg";
        message = "Bad request";
        connection = "close";
    }
    else if (code == 403) {
        statusText = "Forbidden";
        imagePath = "/Assets/403.jpg";
        message = "Access denied";
        connection = "keep-alive";
    }
    else if (code == 404) {
        statusText = "Not Found";
        imagePath = "/Assets/404.jpg";
        message = "Oups... page missing";
        connection = "keep-alive";
    }
	else if (code == 405) {
		statusText = "Method Not Allowed";
		imagePath = "/Assets/405.jpg";
		message = "You can't do that here";
        connection = "keep-alive";
	}
    else if (code == 413) {
        statusText = "Payload Too Large";
        imagePath = "/Assets/413.jpg";
        message = "Body too big ";
        connection = "close";
    }
    else if (code == 414) {
        statusText = "URI Too Long";
        imagePath = "/Assets/414.jpg";
        message = "URL exploded";
        connection = "close";
    }
    else if (code == 415) {
        statusText = "Unsupported Media Type";
        imagePath = "/Assets/415.jpg";
        message = "Format not supported";
        connection = "keep-alive";
    }
    else if (code == 431) {
        statusText = "Request Header Fields Too Large";
        imagePath = "/Assets/431.jpg";
        message = "Headers too big";
        connection = "close";
    }
    else if (code == 500) {
        statusText = "Internal Server Error";
        imagePath = "/Assets/500.jpg";
        message = "Something broke";
        connection = "keep-alive";
    }
    else {
        statusText = "Error";
        imagePath = "/Assets/error.jpg";
        message = "Unknown error";
        connection = "close";
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
        res.headers["Connection"].push_back(connection);
    else
        res.headers["Connection"].push_back("close");

    return res;
}
