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

#ifndef ISOCKET_HPP
#define ISOCKET_HPP

#include "Webserv.hpp"

class ISocket{

	public: 
	int sockClose() = 0;
	int sockbind() = 0;
	int getfd() = 0;
	int setFd(int fd) = 0;

};

#endif
