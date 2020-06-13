#include <iostream>
#include <vector>
#include <thread>
#include <unordered_map>
#include <sys/user.h>
#include "ServerSocket.h"
#include "Jobs.h"
#include "Message.h"

static const int max_thread = 10;
static const int buffer_size = 10000;
std::mutex mutex;
std::mutex mutex_map;
std::mutex mutex_mex;

std::vector<Message> messages;
std::unordered_map<std::string, std::weak_ptr<Socket>> users2sockets;

Jobs jobs(max_thread * 10);

void save_username(std::shared_ptr<Socket> sock, std::string username){
    username.erase(username.end()-2, username.end());
    std::unique_lock lock_cout(mutex);
    std::cout << username << std::endl;
    lock_cout.unlock();
    std::unique_lock lock_(mutex_map);
    if(users2sockets.find(username) != users2sockets.end()){
        lock_.unlock();
        std::string mex = "UsernameUsato\n";
        std::cout << mex.c_str() << " " << sizeof(mex.c_str()[0]) * mex.size() << std::endl;
        sock->write(mex.c_str(), sizeof(mex.c_str()[0]) * mex.size());

    } else{
        sock->setUsername(username);
        users2sockets[sock->getUsername()] = sock;
        lock_.unlock();
        std::string mex = "UsernameRegistrato\n";
        std::cout << mex.c_str() << " " << sizeof(mex.c_str()[0]) * mex.size() << std::endl;
        sock->write(mex.c_str(), sizeof(mex.c_str()[0]) * mex.size());
    }
}

void send_mex(std::shared_ptr<Socket> sock, std::string text, std::string username_from, std::string username_to){
    Message message(username_from, username_to, text);
    std::unique_lock lock(mutex_mex);
    messages.push_back(message);
    lock.unlock();
    sock->write(message.to_string().c_str(), message.to_string().size() * sizeof(message.to_string().c_str()[0]));
}

void send_all(std::shared_ptr<Socket> sock, std::string text){
    for(std::pair<std::string, std::weak_ptr<Socket>> el: users2sockets){
        if(el.first != sock->getUsername()){
            if (auto ptr = el.second.lock()) {
                send_mex(ptr, text, sock->getUsername(), el.first);
            }
        }
    }
}

void worker(){
    char buffer[buffer_size];
    while (true){
        try {
            std::shared_ptr<Socket> sock = jobs.get();
            if(sock == nullptr){
                std::unique_lock lock(mutex);
                lock.unlock();
                if(jobs.producer_is_ended()){
                    std::unique_lock lock(mutex);
                    std::cout << "Exit thread" << std::endl;
                    lock.unlock();
                    return;
                }
            } else {
                ssize_t size = sock->read(buffer, buffer_size);
                if(size > 0){
                    if(sock->getUsername() == ""){
                        save_username(sock, std::string(buffer));
                    } else {
                        send_all(sock, std::string(buffer));
                    }
                    jobs.put(sock);
                }
            }
        }
        catch (std::runtime_error& error) {

        }
    }
}

int main() {
    ServerSocket serverSocket(6003, 10);
    std::vector<std::thread> threads;
    threads.reserve(max_thread);
    for(int i=0; i<max_thread; i++)
        threads.emplace_back(std::thread(worker));
        std::vector<std::shared_ptr<Socket>> sockets;
    while (true) {
        try {
            struct sockaddr addr;
            socklen_t len;
            Socket s = serverSocket.accept(&addr, &len);
            std::cout << "I'm here" << std::endl;
            std::shared_ptr<Socket> ptr = std::make_shared<Socket>(std::move(s));
            jobs.put(ptr);
        }
        catch (std::runtime_error& error) {

        }
    }
    return 0;
}
