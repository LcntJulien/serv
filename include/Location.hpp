/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlecorne <jlecorne@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/13 20:50:08 by jlecorne          #+#    #+#             */
/*   Updated: 2024/04/13 21:25:20 by jlecorne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
# define LOCATION_HPP

#include "Server.hpp"

class Location : public Server
{
private:
public:
    Location();
    Location(const Location &src);
    ~Location();
    Location &operator=(const Location &rhs);

    class Loca_excep : public std::exception {
        public: virtual const char *what() const throw();
    };
};

#endif