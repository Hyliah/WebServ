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

#ifndef SOCKETSERVER_HPP
#define SOCKETSERVER_HPP

#include "Webserv.hpp"
#include "ASocket.hpp"

class SocketServer : public ASocket{

	private:
		std::string					_port;
		std::vector<ServerConfig*>  _servers;

	public:
		SocketServer();
		SocketServer(const SocketServer &other);
		SocketServer& operator=(const SocketServer &other);
		~SocketServer();

		int	sockListen();
		int	sockAccept();

		ServerConfig*	getServeur() const;
		std::string		getPort()const;
		void			setPort(std::string &port);

};

#endif
