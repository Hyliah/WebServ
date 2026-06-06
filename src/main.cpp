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
#include "Exceptions.hpp"
#include "LocationConfig.hpp"
#include "ParserConfig.hpp"
#include "ServerConfig.hpp"
#include <iostream>
#include <csignal>

//WebServer* gSignal = NULL;

//void	testPrintParse();

static WebServer* g_server = 0;

void handle_sigint(int signum) {
    if (signum)
        g_server->_running = false;
}

int main (int ac, char **av)
{
    if (ac != 2) {
        std::cerr << "Usage: ./webserv [config_file]" << std::endl;
        return 1;
    }

    try {
        ParserConfig parser;
        parser.parse(av[1]);

        WebServer webserver(parser.getServers());

        g_server = &webserver;
        signal(SIGINT, handle_sigint);

        webserver.pollLoop();

        g_server = 0; // cleanup propre
    }
    catch (const ParseException &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    catch (const std::exception &e) {
        std::cerr << "Unexpected error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}

















// print les element de la classe pour voir si tt ok
void	testPrintParse()
{
	std::cout << "Test print verif" << std::endl;
	ParserConfig parser;
	parser.parse("config/4-everything.conf");
	const std::vector<ServerConfig> &servers = parser.getServers();
	for (size_t i = 0; i < servers.size(); i++) {
		std::cout << "Server " << i << ":" << std::endl;
		std::cout << "  Port: " << servers[i].port << std::endl
				  << "  Host: " << servers[i].host << std::endl
				  << "  Server Name: " << servers[i].serverName << std::endl
				  << "  Max Body Size: " << servers[i].maxBodySize << std::endl
				  << "  Root: " << servers[i].root << std::endl;
		std::cout << "  Index: ";
        for (size_t idx = 0; idx < servers[i].index.size(); idx++)
            std::cout << servers[i].index[idx] << " ";
        std::cout << std::endl;
		std::cout << "  Error Pages:" << std::endl;
		std::map<int, std::string>::const_iterator itErr;
		for (itErr = servers[i].errorPages.begin(); itErr != servers[i].errorPages.end(); ++itErr) {
			std::cout << "    Code " << itErr->first << " -> " << itErr->second << std::endl;
		}
		// contuinuer avec location apres 
		std::cout << "  Locations:" << std::endl;
		for (size_t j = 0; j < servers[i].locations.size(); j++) {
			std::cout << " Location " << j << ":" << std::endl;
			std::cout << "   Path: " << servers[i].locations[j].path << std::endl
					  << "   Root: " << servers[i].locations[j].root << std::endl
					  << "   Autoindex: " << (servers[i].locations[j].autoindex ? "on" : "off") << std::endl;
			std::cout << "   Index: ";
            for (size_t idx = 0; idx < servers[i].locations[j].index.size(); idx++)
                std::cout << servers[i].locations[j].index[idx] << " ";
            std::cout << std::endl
					  << "   Max Body Size: " << servers[i].locations[j].maxBodySize << std::endl
					  << "   Return URL: " << servers[i].locations[j].returnUrl << std::endl
					  << "   Upload Store: " << servers[i].locations[j].uploadStore << std::endl
					  << "   Methods: ";
			for (size_t k = 0; k < servers[i].locations[j].methods.size(); k++) {
				std::cout << servers[i].locations[j].methods[k] << " ";
			}
			std::cout << std::endl
					  << "   CGI Enabled: " << (servers[i].locations[j].cgiEnabled ? "yes" : "no") << std::endl;
			if (servers[i].locations[j].cgiEnabled) {
				std::cout << "      CGI Handlers:" << std::endl;
				std::map<std::string, std::string>::const_iterator itCgi;
				for (itCgi = servers[i].locations[j].cgiInfo.begin(); itCgi != servers[i].locations[j].cgiInfo.end(); ++itCgi) {
					std::cout << "        Extension: " << itCgi->first << " -> Path: " << itCgi->second << std::endl;
				}
			}
			std::cout << std::endl;
		}
	}
}
