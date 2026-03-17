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

// Ici on peut faire comme avant et stocker toutes les fonctions de test
// qu'on utilise plus mais pour les avoir en back up si jamais 

//VERSION TEST POUR LE PARSER CONFIG AVEC PLEIN DE COUT 

// VERSION TEST BELOW
// void ParserConfig::parse(const std::string &configFilePath) {
// 	// test readfile
// 	std::string content = readFile(configFilePath);
// 	if (content.empty()) {
// 		throw ParseException("Config file is empty: " + configFilePath);
// 	}
// 	else{
// 		std::cout << "Contenu du fichier:" << std::endl;
// 		std::cout << content << std::endl;
// 	}

// 	// test remove comments
// 	removeComments(content);
// 	std::cout << "Contenu après suppression des commentaires:" << std::endl;
// 	std::cout << content << std::endl;

// 	// test tokenize
// 	tokenize(content);
// 	// verif avec print pour debug 
// 	std::cout << "Tokens:" << std::endl;
// 	for (size_t i = 0; i < _tokens.size(); i++) {
// 		std::cout << "Token " << i << ": " << _tokens[i] << std::endl;
// 	}

// 	// Parse server
// 	// boucle pour trouver "server", et appelle parseServeur
// 	// init de l'iterateur 
// 	std::vector<std::string>::iterator it = _tokens.begin();
// 	// parcourir les tokens
// 	while (it != _tokens.end()){
// 		if (*it == "server"){
// 			parseServer(it);
// 		}
// 		else{
// 			throw ParseException("Est ce qu'il y a une erreur ici ou pas ?? rajouter + *it ou pas ? ");
// 		}
// 	}
// }