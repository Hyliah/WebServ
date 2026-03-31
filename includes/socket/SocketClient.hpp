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

#define DEFAULT_MAX_BODY_SIZE 1048576 // 1 MB
#define MAX_HEADER_SIZE 42 //faire mieux
#define MAX_HEADER_LINE_SIZE 42

#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <netdb.h>

#include "../http/HttpRequest.hpp"

enum ChunkState {
    CHUNK_SIZE,
    CHUNK_DATA,
    CHUNK_CRLF,
    CHUNK_DONE,
    CHUNK_ERROR
};


class SocketClient {

	private:

		int         _fd;
		long		_bytesRead;
		long		_bytesPending;
		std::string _buffer; //recupéré avec recv() - attention en plusieurs fois
		ChunkState  _chunkState;

		HttpRequest _request;

		struct sockaddr_storage _addr; 
		
		SocketClient(const SocketClient& other);
		SocketClient& operator=(const SocketClient& other);
		
		
	public:
	
		bool		ignoreBody;
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
		int					getFd();
		HttpRequest&		getRequest();
		const std::string&	getBuffer() const;
		long				getBytes();
		
		//parsing de la request du Paul  
		void	addBytes(long bytes);
		void	appendBuffer(const std::string& str);
		void	parseRequest();
		//void	parseBody(std::string &buffer, size_t &position);
		
		// parsing firstLine & Headers 
		void	parseFirstLine(std::string &buffer, size_t &position);
		void	parseHeaders(std::string &buffer, size_t &position);
		
		//Parsing Body 
		void	parsingNoBody();
		void	parsingChunked();
		void	parsingContentLength();

		// checks Parsing 
		bool	isValidURI();
		bool	isValidMethod();
		bool	isValidVersion();
		bool	isValidBody();
		bool	isDone() const;

		void	defineBodyType();
		void	cleanBuffer();
		// int receiveData();
		// int sendData(const std::string& data);
};

#endif