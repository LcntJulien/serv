#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>
#include <cstdlib> // For popen

const std::string SPATH = "./website/";
const std::string CGIPATH = "./cgi-bin/";
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

// Function to execute CGI script and capture its output
std::string execute_cgi(const std::string& script_name) {
    std::string command = CGIPATH + script_name;
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
                    // Avoid favicon requests
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
                        // Handle POST request
                        // You can extract the requested file name or any other relevant data from the request body
                    }

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

                    // formate response to send it
                    std::stringstream ss;
                    ss << resHeader.str();
                    ss << resBody;
                    std::string response = ss.str();

                    // display response's header
                    std::cout << resHeader.str() << std::endl;

                    // Send HTTP response
                    send(fds[i].fd, response.c_str(), response.length(), 0);
                }
            }
        }
    }
    // Close server socket
    close(serv_socket);
    return 0;
}