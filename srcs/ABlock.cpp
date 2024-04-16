/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ABlock.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlecorne <jlecorne@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 18:16:51 by jlecorne          #+#    #+#             */
/*   Updated: 2024/04/16 18:30:59 by jlecorne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ABlock.hpp"

ABlock::ABlock() {}

ABlock::ABlock(const ABlock &src) {(void)src;}

ABlock::~ABlock() {}

ABlock  &ABlock::operator=(const ABlock &rhs) {
    (void)rhs;
    return *this;
}