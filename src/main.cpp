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

#include "Webserv.hpp"
#include "Exceptions.hpp"

void	testPrint();

int main (int ac, char **av)
{
	if (ac != 2) {
		std::cerr << "Usage: ./webserv [config_file]" << std::endl;
		return 1;
	}
	try {
		ParserConfig parser;
		parser.parse(av[1]);
		testPrint();
	} catch (const ParseException &e) {
		std::cerr << e.what() << std::endl;
		return 1;
	} catch (const std::exception &e) {
		std::cerr << "Unexpected error: " << e.what() << std::endl;
		return 1;
	}
	return 0;
}

// try and catch direct ds le main car sinon ca catch pas l'eereur et ca nous fait un mess d'erreur deguelasse 
// avec 2 ligens de trop 

// print les element de la classe pour voir si tt ok
void	testPrint()
{
	std::cout << "Test print verif" << std::endl;
	ParserConfig parser;
	parser.parse("config/test.conf");
	const std::vector<ServerConfig> &servers = parser.getServers();
	for (size_t i = 0; i < servers.size(); i++) {
		std::cout << "Server " << i << ":" << std::endl;
		std::cout << "  Port: " << servers[i].port << std::endl
				  << "  Host: " << servers[i].host << std::endl
				  << "  Server Name: " << servers[i].serverName << std::endl
				  << "  Max Body Size: " << servers[i].maxBodySize << std::endl
				  << "  Root: " << servers[i].root << std::endl;
		std::cout << "  Error Pages:" << std::endl;
		// contuinuer avec location apres 
		std::cout << "  Locations:" << std::endl;
		for (size_t j = 0; j < servers[i].locations.size(); j++) {
			std::cout << " Location " << j << ":" << std::endl;
			std::cout << "   Path: " << servers[i].locations[j].path << std::endl
					  << "   oot: " << servers[i].locations[j].root << std::endl
					  << "   Autoindex: " << (servers[i].locations[j].autoindex ? "on" : "off") << std::endl
					  << "   Index: " << servers[i].locations[j].index << std::endl
					  << "   Return URL: " << servers[i].locations[j].returnUrl << std::endl
					  << "   Upload Store: " << servers[i].locations[j].uploadStore << std::endl
					  << "   CGI Enabled: " << (servers[i].locations[j].cgiEnabled ? "yes" : "no") << std::endl;

			std::cout << std::endl;
		}
	}
}
