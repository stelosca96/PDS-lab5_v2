////
//// Created by stefano on 13/06/20.
////
//

#include "Socket.h"
#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <bits/stdc++.h>

Socket::Socket(int sockfd) : sockfd(sockfd) {
    std::cout << "Socket " << sockfd << " created" << std::endl;
}

Socket::Socket(){
    username = "";
    sockfd = ::socket(AF_INET, SOCK_STREAM, 0); //solo tcp ipv4
    if (sockfd < 0) throw std::runtime_error("Cannot create socket");
    std::cout << "Socket" << sockfd << " created" << std::endl;
}
Socket::~Socket() {
    if (sockfd != 0) {
        std::cout << "Socket " << sockfd << " closed" << std::endl;
        ::close(sockfd);
    }
}

Socket::Socket(Socket &&other) : sockfd(other.sockfd){  // costruttore di movimento
other.sockfd = 0;
}

Socket& Socket::operator=(Socket &&other){   // costruttore di copia per movimento
    if(sockfd !=0) close(sockfd);
    sockfd = other.sockfd;
    other.sockfd = 0;
    return *this;
}


ssize_t Socket::read(char *buffer, size_t len){
    ssize_t res = recv(sockfd, buffer, len, MSG_NOSIGNAL);
    if(res < 0) throw std::runtime_error("Cannot receive from socket");
    return res;
}

ssize_t Socket::write(const char *buffer, size_t len){
    ssize_t res = send(sockfd, buffer, len, MSG_NOSIGNAL);
    if(res < 0) throw std::runtime_error("Cannot write to socket");
    return res;
}

ssize_t Socket::write_to(const char *buffer, size_t len, int options, struct sockaddr_in *addr, size_t lena){
    ssize_t res = sendto(sockfd, buffer, len, options, reinterpret_cast<struct sockaddr*>(addr), lena);
    return res;
}

void Socket::connect(struct sockaddr_in *addr, unsigned int len){
    if(::connect(sockfd, reinterpret_cast<struct sockaddr*>(addr), len)!=0)
        throw std::runtime_error("Cannot connect to remote socket");
}

const std::string &Socket::getUsername() const {
    return username;
}

void Socket::setUsername(const std::string &username) {
    Socket::username = username;
}
//
//Socket::~Socket() {
//    std::cout << "Closing socket client" << std::endl;
//    ::close(sockfd);
//}
//
//ssize_t Socket::write(void *buffer, size_t n) {
//    ssize_t size = ::write(sockfd, buffer, n);
//    if (size == -1)
//        throw std::runtime_error(strerror(errno));
//    return size;
//}
//
//ssize_t Socket::read(void *buffer, size_t n) {
//    ssize_t size = ::read(sockfd, buffer, n);
//    if (size == -1)
//        throw std::runtime_error(strerror(errno));
//    return size;
//}
//
//
//Socket::Socket(int sockfd) : sockfd(sockfd) {
//}
//
////Socket &Socket::operator=(Socket &&other) {
////    if(sockfd !=0)
////        close(sockfd);
////    sockfd = other.sockfd;
////    other.sockfd = 0;
////    return *this;
////}
//
//Socket::Socket(Socket &&other) : sockfd(other.sockfd){  // costruttore di movimento
//other.sockfd = 0;
//}