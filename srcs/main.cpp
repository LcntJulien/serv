// #include "../include/Request.hpp"
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

const int PORT = 8080;
const std::string SPATH = "./website/";

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
    while (true) {
        // Accept incoming connection
        int client_socket = accept(serv_socket, nullptr, nullptr);
        if (client_socket < 0) {
            std::cerr << "Error accepting connection\n";
            continue;
        }

        // Read HTTP request
        char buffer[1024] = {0};
        read(client_socket, buffer, 1024);
        std::string header(buffer);
        // Ignore favicon request (for now)
        if (header.find("GET /favicon.ico") != std::string::npos) {
            close(client_socket);
            continue;
        }
        std::cout << "\033[90m" << buffer << "\033[0m" << std::endl;

        /*
         - ext function to get method (needed below)
        */

        // get requested file's name
        std::string reqFile;
        size_t pos = header.find("GET ");
        if (pos != std::string::npos) {
            size_t end_pos = header.find(" HTTP/1.1", pos + 4);
            if (end_pos != std::string::npos) {
                reqFile = header.substr(pos + 5, end_pos - pos - 5);
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
                resBody = get_fileContent("./website/public/404.html");
            }
        } else {
            resHeader << "HTTP/1.1 400 Bad Request\r\n\r\n";
            resBody = get_fileContent("./website/public/400.html");
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

        // Send HTTP response && close connection
        send(client_socket, response.c_str(), response.length(), 0);
        close(client_socket);
    }

    return 0;
}