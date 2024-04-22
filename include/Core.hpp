/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Core.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlecorne <jlecorne@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/13 20:52:00 by jlecorne          #+#    #+#             */
/*   Updated: 2024/04/22 18:50:24 by jlecorne         ###   ########.fr       */
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
#include <map>
#include <poll.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "Server.hpp"

const std::string WEBSITE_PATH = "./docs/";
const std::string CGI_PATH = "./cgi-bin/";
const int MAX_CLIENTS = 20;

class Core
{
private:
    std::map<int, int>  _psocket;
    std::vector<int>    _sockets;
    std::vector<int>    _ports;
    std::vector<Server> _clusters;
    int                 _max_clients;

    Core(const Core &src);
    Core &operator=(const Core &rhs);
public:
    Core();
    ~Core();

    size_t  get_ssize();
    int     get_clients();
    int     get_socket(int i);
    int     get_psock(int socket);

    void    close_sock();
    void    bind_ports();
    void    add_port(int port);
    void    add_cluster(const Server &cluster);
    void    set_psock(int socket, int port);

    class CantFindPort : public std::exception {
        public: virtual const char *what() const throw();
    };

    class CantCreateSocket : public std::exception {
        public: virtual const char *what() const throw();
    };

    class CantBindSocket : public std::exception {
        public: virtual const char *what() const throw();
    };

    class ListeningError : public std::exception {
        public: virtual const char *what() const throw();
    };
};

template<typename T>
void    printMsg(T msg, std::string color, int endl) {
    std::cout << color << msg << RESET;
    for (int i = 0; i < endl; i++)
        std::cout << std::endl;
}

std::string handle_cgi_request(const std::string& script_name);
std::string get_fileContent(const std::string& filename);
std::string get_contentType(const std::string& filename);

#endif