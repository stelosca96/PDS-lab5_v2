//
// Created by stefano on 13/06/20.
//

#ifndef PDS_LAB5_SOCKET_H
#define PDS_LAB5_SOCKET_H

#include <sys/types.h>
#include <string>

class Socket {
    int sockfd;
    std::string username;
    Socket(int sockfd);
    Socket(const Socket &) = delete; //elimino il costruttore di copia
    Socket &operator=(const Socket &) = delete; //elimino operatore di assegnazione
    friend class ServerSocket; //è friend perché voglio poter chiamare il suo costruttore privato

public:
    Socket();
    ~Socket();
    Socket(Socket &&other);  // costruttore di movimento
    Socket &operator=(Socket &&other); // costruttore di copia per movimento
    ssize_t read(char *buffer, size_t len);
    ssize_t write(const char *buffer, size_t len);
    ssize_t write_to(const char *buffer, size_t len, int options, struct sockaddr_in *addr, size_t lena);
    void connect(struct sockaddr_in *addr, unsigned int len);

    const std::string &getUsername() const;
    void setUsername(const std::string &username);
};

#endif //PDS_LAB5_SOCKET_H
