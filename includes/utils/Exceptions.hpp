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

// ici on fait une classe de base , et apres classes plus specifiques qui heritent
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

// erreurs de parsing config + http ( donc personnalisé avec soit config ou http ... )
class ParseException : public WebservException {
	public:
		ParseException(const std::string& msg) : WebservException("Parsing Error: " + msg) {}
};

// erreur de socket

// erreur de CGI ?

// erreur de http ? 

// others ?? 

#endif
