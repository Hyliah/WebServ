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

#ifndef UTILSPARSING_HPP
#define UTILSPARSING_HPP

#include <string>

long		stringToLong(const std::string &str);
long		hexToLong(const std::string& hex);
int			stringToInt(const std::string &str); // peut etre utile pour port, erreur http ?? check plus tard 
size_t		parseSize(const std::string &str); // pour parser les tailles avec K, M, G
std::string	toLower(const std::string& str);

#endif