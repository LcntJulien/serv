/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Core.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlecorne <jlecorne@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/13 20:52:00 by jlecorne          #+#    #+#             */
/*   Updated: 2024/04/13 21:29:25 by jlecorne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef Core_HPP
# define Core_HPP

// Colors
#define RED "\033[31m"
#define GREY "\033[90m"
#define BLUE "\033[34m"
#define RESET "\033[0m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"

// Includes
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <poll.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

// TMP
const int PORT = 8080;
const int MAX_CLIENTS = 10;
const std::string WEBSITE_PATH = "./docs/";
const std::string CGI_PATH = "./cgi-bin/";

class Server;

class Core
{
private:
    std::vector<int>    _sockets;
    std::vector<Server> _clusters;
    const int           _max_clients = 10;
public:
    Core();
    Core(const Core &src);
    ~Core();
    Core &operator=(const Core &rhs);

    class Core_excep : public std::exception {
        public: virtual const char *what() const throw();
    };
};

template<typename T>
void    printMsg(T msg, std::string color, int endl) {
    std::cout << color << msg << RESET;
    for (int i = 0; i < endl; i++)
        std::cout << std::endl;
}

#endif