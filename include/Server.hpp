/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlecorne <jlecorne@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/13 20:50:06 by jlecorne          #+#    #+#             */
/*   Updated: 2024/04/16 18:28:43 by jlecorne         ###   ########.fr       */
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
// protected:
//     std::vector<std::string>    _methods;
//     std::vector<std::string>    _cgi_ext;
//     std::string                 _root;
//     std::string                 _index;
//     std::string                 _ret_path;
//     std::string                 _cgi_path;
//     bool                        _autoindex;
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