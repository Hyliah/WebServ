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

#include "Webserv.hpp"
#include "ASocket.hpp"

class SocketClient : public ASocket{

	private:
		std::string		_buffer;
		int				_port;

	public:
		SocketClient();
		SocketClient(int sockFd);
		SocketClient(const SocketClient &other);
		SocketClient& operator=(const SocketClient &other);
		~SocketClient();

		int	sockListen();
		int	sockAccept();

		std::string		getBuffer() const;
		int				getPort() const;
		void			setPort(int port);

};

#endif