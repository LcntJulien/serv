/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlecorne <jlecorne@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 16:37:10 by jlecorne          #+#    #+#             */
/*   Updated: 2024/04/22 18:51:28 by jlecorne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Core.hpp"
#include "../include/Server.hpp"
#include "../include/Location.hpp"

int webserv(Core *serv) {
    serv->bind_ports();
    pollfd fds[serv->get_clients()];
    
    for (size_t i = 0; i < serv->get_ssize(); i++) {
        fds[i].fd = serv->get_socket(i);
        fds[i].events = POLLIN;
    }
    int nfds = serv->get_ssize();

    while (true) {
        int poll_count = poll(fds, nfds, -1);
        if (poll_count < 0) {
            std::cerr << "Poll error\n";
            break;
        }

        // Check for events on server socket
        for (size_t i = 0; i < serv->get_ssize(); i++) {
            if (fds[i].revents & POLLIN) {
                int csocket = accept(fds[i].fd, nullptr, nullptr);
                if (csocket < 0) {
                    std::cerr << "Error accepting connection\n";
                    continue;
                }
                // Add new client socket to fds array
                fds[nfds].fd = csocket;
                fds[nfds].events = POLLIN;
                nfds++;
            }
        }

        // Handle events on client sockets
        for (int i = serv->get_ssize(); i < nfds; i++) {
            if (fds[i].revents & POLLIN) { // Check for incoming data
                char buffer[1024] = {0};
                size_t bytes_received = recv(fds[i].fd, buffer, sizeof(buffer), 0);
                if (bytes_received <= 0) {
                    // Client disconnected or error occurred
                    close(fds[i].fd);
                    // Remove client socket from fds array
                    for (int j = i; j < nfds - 1; j++) {
                        fds[j] = fds[j + 1];
                    }
                    nfds--; // Decrement the number of file descriptors
                } else {
                    std::string request(buffer);
                    printMsg(request, BLUE, 1);
                    /*
                    - ext function to get method (needed below)
                    */

                    // get requested file's name
                    std::string reqFile;
                    size_t get_pos = request.find("GET ");
                    size_t post_pos = request.find("POST ");
                    if (get_pos != std::string::npos) {
                        size_t end_pos = request.find(" HTTP/1.1", get_pos + 4); 
                        if (end_pos != std::string::npos) {
                            reqFile = request.substr(get_pos + 5, end_pos - get_pos - 5);
                        }
                        if (reqFile.empty())
                            reqFile = "index.html";
                    } else if (post_pos != std::string::npos) {
                        // Extract the requested file name from the POST request
                        size_t start_pos = post_pos + strlen("POST ");
                        size_t end_pos = request.find(" HTTP/1.1", start_pos);
                        if (end_pos != std::string::npos) {
                            reqFile = request.substr(start_pos, end_pos - start_pos);
                        }
                    }

                    printMsg(reqFile, YELLOW, 1);

                    // Load requested file
                    std::stringstream resHeader;
                    std::string resBody;

                    // Write appropriate response
                    if (!reqFile.empty()) {
                        // Check if the requested file is a CGI script
                        size_t dotPos = reqFile.find_last_of(".");
                        if (dotPos != std::string::npos) {
                            std::string extension = reqFile.substr(dotPos);
                            if (extension == ".cgi") {
                                // Handle CGI script request
                                std::string cgi_response = handle_cgi_request(reqFile);
                                if (!cgi_response.empty()) {
                                    // Send CGI response to client
                                    send(fds[i].fd, cgi_response.c_str(), cgi_response.length(), 0);
                                    continue; // Skip processing static file requests
                                }
                                else {
                                    resHeader << "HTTP/1.1 404 Not Found\r\n\r\n";
                                    resBody = get_fileContent("./website/status/404.html");
                                    break;
                                }
                            }
                        }
                        std::string filename = WEBSITE_PATH + reqFile;
                        resBody = get_fileContent(filename);
                        if (!resBody.empty()) {
                            resHeader << "HTTP/1.1 200 OK\r\n";
                            resHeader << "Content-Type: " + get_contentType(filename) + "\r\n";
                            resHeader << "Content-Length: " + std::to_string(resBody.size()) + "\r\n";
                            resHeader << "\r\n";
                        } else {
                            resHeader << "HTTP/1.1 404 Not Found\r\n\r\n";
                            resBody = get_fileContent("./docs/status/404.html");
                        }
                    } else {
                        resHeader << "HTTP/1.1 400 Bad Request\r\n\r\n";
                        resBody = get_fileContent("./docs/status/400.html");
                    }

                    /*
                    - Call to ext function for request parsing (use class ?
                    - Write HTTP response (header) (call to ext function to make header depending on request's status (using class might be usefull))
                    - Append requested file or img to the header (might be usefull to use if statement to parse cases)
                    */

                    // formate response to send it
                    std::stringstream ss;
                    ss << resHeader.str();
                    ss << resBody;
                    std::string response = ss.str();

                    // display response's header
                    printMsg(resHeader.str(), YELLOW, 2);

                    // Send HTTP response
                    send(fds[i].fd, response.c_str(), response.length(), 0);
                }
            }
        }
    }
    // Close server sockets
    serv->close_sock();
    return 0;
}

int main(int ac, char **av) {
    (void)av;
    if (ac /*== 2*/)
    {
        try {
            Core *serv = new Core;
            serv->add_port(8000);
            serv->add_port(8001);
            serv->add_port(8002);
            webserv(serv);
        }
        catch (std::exception &e) {
            std::cerr << e.what() << std::endl;
            return 1;
        }
    }
    else {
        std::cerr << GREY << "webserv: " << RED << "ERROR" << GREY << ": Bad argument" << std::endl;
        std::cerr << GREY << "usage: ./webserv [configuration file]" << std::endl;
    }
    return 0;
}