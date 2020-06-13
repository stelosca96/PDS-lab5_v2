//
// Created by stefano on 13/06/20.
//

#include "Message.h"

Message::Message(const std::string &from, const std::string &to, const std::string &text) : from(from), to(to),
                                                                                            text(text) {}

std::string Message::to_string() {
    return from + ">>" + text;
}
