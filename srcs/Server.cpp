/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlecorne <jlecorne@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/13 21:18:50 by jlecorne          #+#    #+#             */
/*   Updated: 2024/04/16 18:31:56 by jlecorne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Server.hpp"

Server::Server() {
    (void)this->_max_bsize;
}

Server::Server(const Server &src) {
    (void)src;
}

Server::~Server() {
}

Server  &Server::operator=(const Server &rhs) {
    (void)rhs;
    return *this;
}

const char *Server::Serv_excep::what() const throw() {
    return ("");
}