// #ifndef REQUEST_HPP
// # define REQUEST_HPP

// #include <iostream>
// #include <sstream>
// #include <string>
// #include <unistd.h>
// #include <sys/socket.h>
// #include <netinet/in.h>

// #define GET     1
// #define POST    2
// #define PUT     3
// #define DELETE  4

// class Request
// {
// private:
//     int         _method;
//     int         _status_code;
//     long        _content_len;
//     std::string _status_msg;
//     std::string _content_type;
//     std::string _body;
// public:
//     Request();
//     Request(const Request &src);
//     ~Request();

//     Request &operator=(const Request &rhs);

//     class Excep : public std::exception {
//         public: virtual const char *what() const throw();
//     };
// };

// #endif