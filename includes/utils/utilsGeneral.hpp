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
#include <vector>

long		stringToLong(const std::string &str);
long		hexToLong(const std::string& hex);
std::string longToString(long n);
int			stringToInt(const std::string &str); // peut etre utile pour port, erreur http ?? check plus tard 
size_t		parseSize(const std::string &str); // pour parser les tailles avec K, M, G
std::string	toLower(const std::string& str);
bool        isHex(char c);
int         hexValue(char c);
char        hexToChar(char a, char b);
std::string trim(const std::string& s);
void        safeClose(int *fd);
void	    *freePtr(void **ptr);
void	    freeMidTab(char ***tab, int i);
void	    freeTab(char ***tab);

std::vector<std::string> splitLines(std::string str);

#endif