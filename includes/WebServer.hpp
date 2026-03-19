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

#ifndef WEBSERVER_HPP
#define WEBSERVER_HPP

#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "SocketClient.hpp"
#include "SocketServer.hpp"

class WebServer{

	private :
		std::vector<SocketServer>	_socketServers;
		std::map<int, SocketClient>	_socketClients;
		std::vector<ServerConfig>	_servers;
		//vecteurs de Pauls (Polls)

	public :
		WebServer(const std::vector<ServerConfig> &servers);
		WebServer(const WebServer &other) = delete;
		WebServer& operator=(const WebServer &other) = delete;
		~WebServer();

		//gettes & setters
		// SocketServer&	getServer(size_t idx);
		// SocketClient&	getClient(int fd);
		// void	setSocket(SocketServer& socket);
		// void 	setClient(int fd, SocketClient& client);
		// // or
		// void 	addClient(int fd); // faire la construction du Client directement dans cette fonction
		
		
		void	cpyLinkConfig();
		
		// autres
		//void	removeClient(int fd);
		void	initSockets(); // faire une boucle while qui va créer tous les socket. 

		void handleRequest(SocketClient& client); //fonction qui va démarrer le parsing du http dans la classe SocketClient
		//paul loop ?***
	

};

#endif



// ** POLL LOOP : 

/*
	server socket ready -> deja créé dans le parsing de conf -> accept + creation du socketClient

	client socket ready -> recv() -> buffer += data -> parsing HTTP
	-> faire une class pour les Polls et faire un vecteur de cette classe

		for (size_t i = 0; i < _servers.size(); i++)
		{
			pollfd p;
			p.fd = _servers[i].getFd();
			p.events = POLLIN;
			p.revents = 0;

			_pollfds.push_back(p);
		}

		dans la poll loop :
			- accept()
			- recv()
			- parser HTTP
			- send response


*/