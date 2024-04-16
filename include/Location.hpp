/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlecorne <jlecorne@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/13 20:50:08 by jlecorne          #+#    #+#             */
/*   Updated: 2024/04/16 18:18:18 by jlecorne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
# define LOCATION_HPP

#include "ABlock.hpp"

class Location : public ABlock
{
private:
    std::string _path;
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