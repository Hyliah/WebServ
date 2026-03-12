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

#ifndef SERVERCONFIG_HPP
#define SERVERCONFIG_HPP

#include "Webserv.hpp"
#include "LocationConfig.hpp"

// struc server config ?
class ServerConfig {
public:
    int                         port;
    std::string                 host;         // par ex par defaut "0.0.0.0"
    std::string                 server_name;
    size_t                      max_body_size;
    std::map<int, std::string>  error_pages;  // Code (404) -> Chemin du fichier
    std::vector<LocationConfig> locations;

    ServerConfig() : port(8080), max_body_size(1000000) {}
};

#endif