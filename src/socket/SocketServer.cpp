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



// bool SocketServer::bindSocket()
// {
//     struct addrinfo hints;
//     struct addrinfo* res;

//     memset(&hints, 0, sizeof(hints));
//     hints.ai_family = AF_INET;
//     hints.ai_socktype = SOCK_STREAM;
//     hints.ai_flags = AI_PASSIVE;

//     if (getaddrinfo(NULL, _port.c_str(), &hints, &res) != 0)
//         return false;

//     if (bind(_fd, res->ai_addr, res->ai_addrlen) < 0)
//     {
//         freeaddrinfo(res);
//         return false;
//     }

//     freeaddrinfo(res);
//     return true;
// }