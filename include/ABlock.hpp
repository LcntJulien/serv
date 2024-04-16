/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ABlock.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlecorne <jlecorne@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 18:07:19 by jlecorne          #+#    #+#             */
/*   Updated: 2024/04/16 18:18:47 by jlecorne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ABLOCK_HPP
# define ABLOCK_HPP

#include <vector>
#include <string>

class ABlock
{
protected:
    std::vector<std::string>    _methods;
    std::vector<std::string>    _cgi_ext;
    std::string                 _root;
    std::string                 _index;
    std::string                 _ret_path;
    std::string                 _cgi_path;
    bool                        _autoindex;
public:
    ABlock();
    ABlock(const ABlock &src);
    virtual ~ABlock() = 0;
    ABlock &operator=(const ABlock &rhs);
};

#endif