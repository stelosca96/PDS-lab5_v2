//
// Created by stefano on 13/06/20.
//

#ifndef PDS_LAB5_REBOOT_MESSAGE_H
#define PDS_LAB5_REBOOT_MESSAGE_H


#include <string>

class Message {
private:
    std::string from;
    std::string to;
    std::string text;
public:
    Message(const std::string &from, const std::string &to, const std::string &text);
    std::string to_string();
};


#endif //PDS_LAB5_REBOOT_MESSAGE_H
