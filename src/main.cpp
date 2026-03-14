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

int main (int ac, char **av)
{
	if (ac != 2) {
		std::cerr << "Usage: ./webserv [config_file]" << std::endl;
		return 1;
	}
	// test si fonction read file fonctionne
	ParserConfig parser;
	parser.parse(av[1]);
	return 0;
}
