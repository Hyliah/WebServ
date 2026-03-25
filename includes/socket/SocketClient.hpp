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

#ifndef SOCKETCLIENT_HPP
#define SOCKETCLIENT_HPP

#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <netdb.h>

#include "../http/HttpRequest.hpp"

class SocketClient {

	private:

		int         _fd;
		std::string _buffer; //recupéré avec recv() - attention en plusieurs fois

		HttpRequest _request;

		struct sockaddr_storage _addr; 
		
		SocketClient(const SocketClient& other);
		SocketClient& operator=(const SocketClient& other);
		
	public:
	
		bool        headerParsed;
		bool        requestCompleted;
		bool        contentLength;
		bool        chunked;

		// construtor & destructors 
		SocketClient();
		SocketClient(int fd, struct sockaddr_storage addr);
		~SocketClient();
		
		void closeSocket();
		
		// Getters and setters
		int					getFd() const;
		HttpRequest&		getRequest();
		const std::string&	getBuffer() const;
		
		//parsing de la request du Paul  
		void	appendBuffer(const std::string& str);
		void	parseRequest();
		//void	parseBody(std::string &buffer, size_t &position);
		
		// parsing firstLine & Headers 
		void	parseFirstLine(std::string &buffer, size_t &position);
		void	parseHeaders(std::string &buffer, size_t &position);
		
		//Parsing Body 
		void	parsingNoBody(){}
		void	parsingChunked(){}
		void	parsingContentLength(){}

		// checks Parsing 
		bool	isValidURI();
		bool	isValidMethod();
		bool	isValidVersion();

		void handleLength();
		// int receiveData();
		// int sendData(const std::string& data);
};

#endif