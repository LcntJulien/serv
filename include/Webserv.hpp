#ifndef WEBSERV_HPP
# define WEBSERV_HPP

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <poll.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

// Colors
#define RED "\033[31m"
#define GREY "\033[90m"
#define BLUE "\033[34m"
#define RESET "\033[0m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"

const int PORT = 8080;
const int MAX_CLIENTS = 10;
const std::string WEBSITE_PATH = "./docs/";
const std::string CGI_PATH = "./cgi-bin/";

// class Webserv
// {
// private:
//     const int           port;
//     const int           max_clients;
//     const std::string   ws_path;
//     const std::string   cgi_path;
// public:
//     Webserv();
//     Webserv(const Webserv &src);
//     ~Webserv();

//     Webserv &operator=(const Webserv &rhs);

//     class Excep : public std::exception {
//         public: virtual const char *what() const throw();
//     };
// };

template<typename T>
void    printMsg(T msg, std::string color, int endl) {
    std::cout << color << msg << RESET;
    for (int i = 0; i < endl; i++)
        std::cout << std::endl;
}

#endif