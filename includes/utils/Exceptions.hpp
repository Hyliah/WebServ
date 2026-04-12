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

#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP

#include <stdexcept>
#include <string>

#include <exception>
#include <string>
#include <sstream>

// Classe de base
class WebservException : public std::exception {
protected:
    std::string _message;

public:
    WebservException(const std::string& msg) : _message(msg) {}
    virtual ~WebservException() throw() {}
    virtual const char* what() const throw() {
        return _message.c_str();
    }
};

// Enum classique compatible C++98
enum ParseType {
    CONF,
    HTTP,
    CGI
};

// Fonction utilitaire pour convertir l'enum en string
inline const char* parseTypeToString(ParseType type) {
    switch (type) {
        case CONF: return "CONF";
        case HTTP: return "HTTP";
        case CGI:  return "CGI";
        default:   return "UNKNOWN";
    }
}

// Exception de parsing
class ParseException : public WebservException {
public:
    ParseException(ParseType type, const std::string& msg)
        : WebservException(buildMessage(type, msg)) {}

private:
    static std::string buildMessage(ParseType type, const std::string& msg) {
        std::ostringstream oss;
        oss << "Parsing " << parseTypeToString(type) << " Error: " << msg;
        return oss.str();
    }
};


/*

int main() {
    try {
        throw ParseException(HTTP, "Invalid HTTP header");
    } catch (const WebservException& e) {
        std::cout << e.what() << std::endl;
    }
    return 0;
}

*/
// erreur de socket

class SocketException : public WebservException {
	public:
		SocketException(const std::string& msg)
			: WebservException(buildMessage(msg)) {}

	private:
		static std::string buildMessage(const std::string& msg) {
			std::ostringstream oss;
			oss << "Socket Error: " << msg;
			return oss.str();
		}
};

class RunningException : public WebservException {
	public:
		RunningException(const std::string& msg)
			: WebservException(buildMessage(msg)) {}

	private:
		static std::string buildMessage(const std::string& msg) {
			std::ostringstream oss;
			oss << "Running Error: " << msg;
			return oss.str();
		}
};


// erreur de CGI ?

// erreur de http ? 

// others ?? 

#endif
