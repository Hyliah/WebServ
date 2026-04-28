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

#include "utilsGeneral.hpp"
#include "WebServer.hpp"
#include "Exceptions.hpp"
#include <climits>


/* ************************************************** */
/* Conversion					                      */
/* ************************************************** */


/*
CHANGER POUR LE ENUM !!!!!!!!!!!!!!!!!
*/
long	stringToLong(const std::string &str){
	long result = 0;
	for (size_t i = 0; i < str.size(); i++)
	{
		if (!isdigit(static_cast<unsigned char>(str[i])))
			throw ParseException(CONF, "Invalid number: " + str);
		result = result * 10 + (str[i] - '0');
	}
	return result;
}

std::string longToString(long n) {
    std::stringstream ss;
    ss << n;
    return ss.str();
}

long	hexToLong(const std::string& hex)
{
    long result = 0;
    
    for (size_t i = 0; i < hex.size(); i++)
    {
        char c = hex[i];
        result *= 16;
        
        if (c >= '0' && c <= '9')
            result += c - '0';
        else if (c >= 'a' && c <= 'f')
            result += c - 'a' + 10;
        else if (c >= 'A' && c <= 'F')
            result += c - 'A' + 10;
        else
            return -1;
    }
    return result;
}

char hexToChar(char a, char b) {
    return (hexValue(a) * 16 + hexValue(b));
}


int	stringToInt(const std::string &str){
	// On peut faire la meme chose que pour stringToLong mais en utilisant un int 
	// verif les limites de l'int pour eviter les overflow
	int result = 0;
	for (size_t i = 0; i < str.size(); i++)
	{
		if (!isdigit(static_cast<unsigned char>(str[i])))
			throw ParseException(CONF, "Invalid number: " + str);
		if (result > (INT_MAX - (str[i] - '0')) / 10) // Verifie que le prochain chiffre n'entraînera pas un overflow
			throw ParseException(CONF, "Number too large: " + str);
		result = result * 10 + (str[i] - '0');
	}
	return result;
}


/* ************************************************** */
/* Autre						                      */
/* ************************************************** */

std::string toLower(const std::string& str){
    std::string res;
    for (size_t i = 0; i < str.size(); ++i) {
        res += std::tolower(str[i]);
    }
	return res;
}

//attention fonction qui fait pas de verif
bool isHex(char c) {
    return (
        (c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f')
    );
}

//attention fonction qui fait pas de verif
int hexValue(char c){
	if (c >= '0' && c <= '9')
		return (c - '0');
	
	else if (c >= 'A' && c <= 'F')
		return (c - 'A' + 10);

	else if (c >= 'a' && c <= 'f')
		return (c - 'a' + 10);

	return -1 ;
} 

size_t	parseSize(const std::string &str){
	if (str.empty()) return 0;
	char unit = str[str.size() - 1]; // pour acceder au dernier caractere si c'est une lettre 
	std::string valueStr = str; // on va faire un copie de ka str
	size_t multiplier = 1;
	// donc si le dernier char est pas un digit ( donc k, m ou g )
	if (!isdigit(unit)) {
		valueStr = str.substr(0, str.size() - 1); // on stock les number en enlevant le dernier char 
		if (unit == 'K' || unit == 'k') multiplier = 1024;
		else if (unit == 'M' || unit == 'm') multiplier = 1024 * 1024;
		else if (unit == 'G' || unit == 'g') multiplier = 1024 * 1024 * 1024;
		else throw ParseException(CONF, "Invalid size unit: " + str);
	}
	// Conversion de la partie numérique 
	long val = stringToLong(valueStr);
	if (val < 0) 
		throw ParseException(CONF, "Size cannot be negative: " + str);
	return static_cast<size_t>(val * multiplier);
}


std::string trim(const std::string& s)
{
    size_t start = 0;
    while (start < s.size() && std::isspace(s[start]))
        start++;

    size_t end = s.size();
    while (end > start && std::isspace(s[end - 1]))
        end--;

    return s.substr(start, end - start);
}

/* ************************************************** */
/* Close and free				                      */
/* ************************************************** */


void	safeClose(int *fd){
	if (!*fd || !fd)
		return;
	if (*fd != -1){
		close(*fd);
		*fd = -1;
	}
}

void	*freePtr(void **ptr)
{
	if (*ptr && ptr)
	{
		free(*ptr);
		*ptr = NULL;
	}
	return (NULL);
}

void	freeMidTab(char ***tab, int i) // supprimer partout
{
	if (!tab && !*tab)
		return ;
	while (i > 0)
	{
		i--;
		freePtr((void **)&((*tab)[i]));
	}
	free(*tab);
	*tab = NULL;
}

void freeTab(char*** envp)
{
    for (int i = 0; (*envp)[i] != NULL; i++)
        free((*envp)[i]);

    delete[] *envp;
    *envp = NULL;
}

std::vector<std::string> splitLines(std::string str){
	std::vector<std::string> ret;
	
	size_t posStart = 0;
    size_t pos;
	
	while((pos = str.find('\n', posStart)) != std::string::npos){
	
		std::string line = str.substr(posStart, pos - posStart);
		if (!line.empty() && line[line.size() - 1] == '\r')
            line.erase(line.size() - 1);

        ret.push_back(line);
        posStart = pos + 1;
    }

    if (posStart < str.size())
    {
        std::string line = str.substr(posStart);
        if (!line.empty() && line[line.size() - 1] == '\r')
            line.erase(line.size() - 1);

        ret.push_back(line);
    }

	return ret;
}