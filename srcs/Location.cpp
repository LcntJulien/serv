/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlecorne <jlecorne@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/13 21:18:59 by jlecorne          #+#    #+#             */
/*   Updated: 2024/04/16 18:31:17 by jlecorne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Location.hpp"

Location::Location() {
}

Location::Location(const Location &src) {
    (void)src;
}

Location::~Location() {
}

Location    &Location::operator=(const Location &rhs) {
    (void)rhs;
    return *this;
}

const char *Location::Loca_excep::what() const throw() {
    return ("");
}