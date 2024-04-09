// // #include "../include/Request.hpp"
// #include <iostream>
// #include <sstream>
// #include <fstream>
// #include <string>
// #include <cstring>
// #include <unistd.h>
// #include <sys/socket.h>
// #include <netinet/in.h>

// const int PORT = 8080;
// const std::string SPATH = "./website/";

// std::string get_fileContent(const std::string& filename) {
//     std::ifstream in(filename, std::ios::in | std::ios::binary);
//     if (in) {
//         std::ostringstream content;
//         content << in.rdbuf();
//         in.close();
//         return content.str();
//     }
//     return "";
// }

// std::string get_contentType(const std::string& filename) {
//     if (filename.find(".html") != std::string::npos) {
//         return "text/html";
//     } else if (filename.find(".css") != std::string::npos) {
//         return "text/css";
//     } else if (filename.find(".js") != std::string::npos) {
//         return "application/javascript";
//     } else {
//         return "application/octet-stream"; // Default to binary data if content type is unknown
//     }
// }

// int main() {
//     // Create socket
//     int serv_socket = socket(AF_INET, SOCK_STREAM, 0);
//     if (serv_socket < 0) {
//         std::cerr << "Error creating socket\n";
//         return 1;
//     }

//     // Bind socket to port
//     sockaddr_in address;
//     address.sin_family = AF_INET;
//     address.sin_addr.s_addr = INADDR_ANY;
//     address.sin_port = htons(PORT);
//     if (bind(serv_socket, (sockaddr*)&address, sizeof(address)) < 0) {
//         std::cerr << "Error binding to port\n";
//         return 1;
//     }

//     // Listen for connections
//     if (listen(serv_socket, 10) < 0) {
//         std::cerr << "Error listening\n";
//         return 1;
//     }

//     std::cout << "\033[1m\033[90mServer listening on port \033[32m" << PORT << "\033[90m\033[0m" << std::endl << std::endl;
//     while (true) {
//         // Accept incoming connection
//         int client_socket = accept(serv_socket, nullptr, nullptr);
//         if (client_socket < 0) {
//             std::cerr << "Error accepting connection\n";
//             continue;
//         }

//         // Read HTTP request
//         char buffer[1024] = {0};
//         read(client_socket, buffer, 1024);
//         std::string header(buffer);
//         // Ignore favicon request (for now)
//         if (header.find("GET /favicon.ico") != std::string::npos) {
//             close(client_socket);
//             continue;
//         }
//         std::cout << "\033[90m" << buffer << "\033[0m" << std::endl;

//         /*
//          - ext function to get method (needed below)
//         */

//         // get requested file's name
//         std::string reqFile;
//         size_t pos = header.find("GET ");
//         if (pos != std::string::npos) {
//             size_t end_pos = header.find(" HTTP/1.1", pos + 4); 
//             if (end_pos != std::string::npos) {
//                 reqFile = header.substr(pos + 5, end_pos - pos - 5);
//             }
//             if (reqFile.empty())
//                 reqFile = "index.html";
//         }

//         // Load requested file
//         std::stringstream resHeader;
//         std::string resBody;

//         // Write appropriate header
//         if (!reqFile.empty()) {
//             std::string filename = SPATH + reqFile;
//             resBody = get_fileContent(filename);
//             if (!resBody.empty()) {
//                 resHeader << "HTTP/1.1 200 OK\r\n";
//                 resHeader << "Content-Type: " + get_contentType(filename) + "\r\n";
//                 resHeader << "Content-Length: " + std::to_string(resBody.size()) + "\r\n";
//                 resHeader << "\r\n";
//             } else {
//                 resHeader << "HTTP/1.1 404 Not Found\r\n\r\n";
//                 resBody = get_fileContent("./website/status/404.html");
//             }
//         } else {
//             resHeader << "HTTP/1.1 400 Bad Request\r\n\r\n";
//             resBody = get_fileContent("./website/status/400.html");
//         }

//         /*
//          - Call to ext function for request parsing (use class ?
//          - Write HTTP response (header) (call to ext function to make header depending on request's status (using class might be usefull))
//          - Append requested file or img to the header (might be usefull to use if statement to parse cases)
//         */

//         std::stringstream ss;
//         ss << resHeader.str();
//         ss << resBody;
//         std::string response = ss.str();

//         std::cout << resHeader.str() << std::endl;

//         // Send HTTP response && close connection
//         send(client_socket, response.c_str(), response.length(), 0);
//         close(client_socket);
//     }

//     return 0;
// }


// ***********************************************************************************

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h> // Include for poll

const std::string SPATH = "./website/";
const int MAX_CLIENTS = 10;
const int PORT = 8080;

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

int main() {
    // Create socket
    int serv_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (serv_socket < 0) {
        std::cerr << "Error creating socket\n";
        return 1;
    }

    // Bind socket to port
    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    if (bind(serv_socket, (sockaddr*)&address, sizeof(address)) < 0) {
        std::cerr << "Error binding to port\n";
        return 1;
    }

    // Listen for connections
    if (listen(serv_socket, 10) < 0) {
        std::cerr << "Error listening\n";
        return 1;
    }
    std::cout << "\033[1m\033[90mServer listening on port \033[32m" << PORT << "\033[90m\033[0m" << std::endl << std::endl;
    
    // Array to hold file descriptors for sockets
    pollfd fds[MAX_CLIENTS]; // Adjust the size as needed

    // Initialize the first element with the server socket
    fds[0].fd = serv_socket;
    fds[0].events = POLLIN; // Listen for incoming data

    int nfds = 1; // Number of file descriptors

    while (true) {
        // Wait for events on any of the sockets
        int poll_count = poll(fds, nfds, -1);
        if (poll_count < 0) {
            std::cerr << "Poll error\n";
            break;
        }

        // Check for events on server socket
        if (fds[0].revents & POLLIN) {
            // Accept incoming connection
            int client_socket = accept(serv_socket, nullptr, nullptr);
            if (client_socket < 0) {
                std::cerr << "Error accepting connection\n";
                continue;
            }
            std::cout << "New client connected" << std::endl;

            // Add new client socket to fds array
            fds[nfds].fd = client_socket;
            fds[nfds].events = POLLIN; // Listen for incoming data
            nfds++; // Increment the number of file descriptors
        }

        // Handle events on client sockets
        for (int i = 1; i < nfds; i++) {
            if (fds[i].revents & POLLIN) { // Check for incoming data
                char buffer[1024] = {0};
                ssize_t bytes_received = recv(fds[i].fd, buffer, sizeof(buffer), 0);
                if (bytes_received <= 0) {
                    // Client disconnected or error occurred
                    std::cout << "Client disconnected" << std::endl;
                    close(fds[i].fd);
                    // Remove client socket from fds array
                    for (int j = i; j < nfds - 1; j++) {
                        fds[j] = fds[j + 1];
                    }
                    nfds--; // Decrement the number of file descriptors
                } else {
                    std::string request(buffer);
                    if (request.find("GET /favicon.ico") != std::string::npos) {
                        close(fds[i].fd);
                        continue;
                    }
                    /*
                    - ext function to get method (needed below)
                    */

                    // get requested file's name
                    std::string reqFile;
                    size_t pos = request.find("GET ");
                    if (pos != std::string::npos) {
                        size_t end_pos = request.find(" HTTP/1.1", pos + 4); 
                        if (end_pos != std::string::npos) {
                            reqFile = request.substr(pos + 5, end_pos - pos - 5);
                        }
                        if (reqFile.empty())
                            reqFile = "index.html";
                    }

                    // Load requested file
                    std::stringstream resHeader;
                    std::string resBody;

                    // Write appropriate header
                    if (!reqFile.empty()) {
                        std::string filename = SPATH + reqFile;
                        resBody = get_fileContent(filename);
                        if (!resBody.empty()) {
                            resHeader << "HTTP/1.1 200 OK\r\n";
                            resHeader << "Content-Type: " + get_contentType(filename) + "\r\n";
                            resHeader << "Content-Length: " + std::to_string(resBody.size()) + "\r\n";
                            resHeader << "\r\n";
                        } else {
                            resHeader << "HTTP/1.1 404 Not Found\r\n\r\n";
                            resBody = get_fileContent("./website/status/404.html");
                        }
                    } else {
                        resHeader << "HTTP/1.1 400 Bad Request\r\n\r\n";
                        resBody = get_fileContent("./website/status/400.html");
                    }

                    /*
                    - Call to ext function for request parsing (use class ?
                    - Write HTTP response (header) (call to ext function to make header depending on request's status (using class might be usefull))
                    - Append requested file or img to the header (might be usefull to use if statement to parse cases)
                    */

                    std::stringstream ss;
                    ss << resHeader.str();
                    ss << resBody;
                    std::string response = ss.str();

                    std::cout << resHeader.str() << std::endl;

                    // Send HTTP response && close connection
                    send(fds[i].fd, response.c_str(), response.length(), 0);
                }
            }
        }
    }

    // Close server socket
    close(serv_socket);

    return 0;
}