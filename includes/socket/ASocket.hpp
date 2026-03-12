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

#ifndef ASOCKET_HPP
#define ASOCKET_HPP

#include "Webserv.hpp"
#include "ISocket.hpp"

class ASocket : public ISocket{

	protected:
		int	_sockFd;
	
	public:
		ASocket();
		ASocket(int sockFd);
		ASocket(const ASocket &other);
		ASocket& operator=(const ASocket &other);
		~ASocket();

		int	sockConnect() = 0;
		void disconnect();

};

#endif