/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Core.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlecorne <jlecorne@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/13 20:51:48 by jlecorne          #+#    #+#             */
/*   Updated: 2024/04/22 18:50:12 by jlecorne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Core.hpp"

Core::Core() : _max_clients(10) {
    (void)this->_max_clients;
}

Core::Core(const Core &src) : _max_clients(src._max_clients) {}

Core::~Core() {}

Core &Core::operator=(const Core &rhs) {
    if (this != &rhs) {
        this->_max_clients = rhs._max_clients;
        this->_clusters = rhs._clusters;
    }
    return *this;
}

size_t  Core::get_ssize() {
    return this->_sockets.size();
}

int     Core::get_clients() {
    return this->_max_clients;
}

int     Core::get_socket(int i) {
    return this->_sockets[i];
}

//renvoit le port mappé sur socket
int     Core::get_psock(int socket) {
    if (_psocket.empty() || _psocket.find(socket) == _psocket.end())
        throw CantFindPort();
    else {
        return _psocket[socket];
    }
}

void    Core::close_sock() {
    for (size_t i = 0; i < this->_sockets.size(); i++) {
        close(this->_sockets[i]);
    }
}

void    Core::add_port(int port) {
    //add check
    this->_ports.push_back(port);
}

//check et ajoute un cluster au vector _culsters (parsing)
void    Core::add_cluster(const Server &cluster) {
    //add check
    this->_clusters.push_back(cluster);
}

//mappe un port sur un socket
void    Core::set_psock(int socket, int port) {
    //add check
    this->_psocket[socket] = port;
}

//bind les ports et push les sockets dans _sockets
void    Core::bind_ports() {
    for (size_t i = 0; i < this->_ports.size(); i++) {
        int sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock < 0)
            throw Core::CantCreateSocket();
        // Bind socket to port
        sockaddr_in address;
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(this->_ports[i]);
        if (bind(sock, (sockaddr*)&address, sizeof(address)) < 0)
            throw Core::CantBindSocket();
        // Listen for connections
        if (listen(sock, 10) < 0)
            throw Core::ListeningError();
        // Message
        std::cout << "\033[1m\033[90mServer listening on ";
        for (size_t i = 0; i < this->_ports.size(); i++) {
            std::cout << "\033[32m" << this->_ports[i] << "\033[0m";
            if (i + 1 == this->_ports.size())
                std::cout << std::endl << std::endl;
        }
        this->_sockets.push_back(sock);
    }
}

const char *Core::CantFindPort::what() const throw() {
    return ("\033[31mERROR\033[0m: Can't find port for this socket");
}

const char *Core::CantCreateSocket::what() const throw() {
    return ("\033[31mERROR\033[0m: Can't create socket");
}

const char *Core::CantBindSocket::what() const throw() {
    return ("\033[31mERROR\033[0m: Can't bind the socket to port");
}

const char *Core::ListeningError::what() const throw() {
    return ("\033[31mERROR\033[0m: Failed listening on port");
}