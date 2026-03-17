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

#include "utilsParsing.hpp"
#include "Exceptions.hpp"

long	stringToLong(const std::string &str){
	long result = 0;
	for (size_t i = 0; i < str.size(); i++)
	{
		if (!isdigit(str[i]))
			throw ParseException("Invalid number: " + str);
		result = result * 10 + (str[i] - '0');
	}
	return result;
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
		else throw ParseException("Invalid size unit: " + str);
	}
	// Conversion de la partie numérique 
	long val = stringToLong(valueStr);
	if (val < 0) 
		throw ParseException("Size cannot be negative: " + str);
	return static_cast<size_t>(val * multiplier);
}