#pragma once

#include <cctype>
#include <ctime>
#include <random>
#include <stdexcept>
#include <string>
#include <unordered_map>

using str = std::string;

const std::unordered_map<unsigned short, str> errtable{
    {0x0001, "0x0001 Empty ID"},
    {0x0002, "0x0002 The ID includes invalid character(s)"},
    {0x0003, "0x0003 Invalid color code; type `help --color-code` for more info"},
    {0x0004, "0x0004 No such element in this kit"},
    {0x0005, "0x0005 Invalid position"},
    {0x0006, "0x0006 Failed to parse JSON"}};
class errcode : public std::runtime_error {
  public:
    errcode(const unsigned short code) : std::runtime_error(errtable.at(code)) {}
    errcode(const str &msg) : std::runtime_error("0x0000 Extern Error: " + msg) {}
};

bool isValidID(const str &id) {
    for (const auto it : id) {
        if (!isalnum(it) && it != '_')
            return false;
    }
    return true;
}

str randomID() {
    std::random_device rd;
    str                o;
    std::mt19937_64    r(time(nullptr) + rd());
    for (int i = 0; i < 50; i++) {
        unsigned r_ = r() % 26;
        if (r_ < 25) {
            o.push_back((char)'a' + r_);
        } else {
            o.push_back('_');
        }
    }
    return o;
}
