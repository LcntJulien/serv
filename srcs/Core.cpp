/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Core.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlecorne <jlecorne@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/13 20:51:48 by jlecorne          #+#    #+#             */
/*   Updated: 2024/04/13 21:00:10 by jlecorne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Core.hpp"

Core::Core() {
}

Core::Core(const Core &src) {
}

Core::~Core() {
}

Core &Core::operator=(const Core &rhs) {

}

const char *Core::Excep::what() const throw() {
    return ("");
}

std::string get_fileContent(const std::string& filename) {
    std::ifstream in(filename, std::ios::in | std::ios::binary);
    if (in) {
        std::ostringstream content;
        content << in.rdbuf();
        in.close();
        return content.str();
    }
    return "";
}

std::string get_contentType(const std::string& filename) {
    if (filename.find(".html") != std::string::npos) {
        return "text/html";
    } else if (filename.find(".css") != std::string::npos) {
        return "text/css";
    } else if (filename.find(".js") != std::string::npos) {
        return "application/javascript";
    } else {
        return "application/octet-stream"; // Default to binary data if content type is unknown
    }
}

// Function to execute CGI script and capture its output
std::string execute_cgi(const std::string& script_name) {
    std::string command = CGI_PATH + script_name;
    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) {
        return "Error executing CGI script";
    }

    char buffer[1024];
    std::string result;
    while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
        result += buffer;
    }
    pclose(pipe);

    return result;
}

// Function to handle CGI requests
std::string handle_cgi_request(const std::string& script_name) {
    return execute_cgi(script_name);
}