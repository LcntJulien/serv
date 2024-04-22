/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlecorne <jlecorne@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/13 20:50:06 by jlecorne          #+#    #+#             */
/*   Updated: 2024/04/22 18:55:46 by jlecorne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include <iostream>
#include <vector>

#include "ABlock.hpp"
#include "Location.hpp"

class Server : public ABlock
{
private:
    std::vector<int>            _ports;
    std::vector<Location>       _locations;
    std::string                 _host;
    std::string                 _serv_name;
    std::string                 _err_path;
    long                        _max_bsize;
public:
    Server();
    Server(const Server &src);
    ~Server();
    Server &operator=(const Server &rhs);

    class Serv_excep : public std::exception {
        public: virtual const char *what() const throw();
    };
};

#endif