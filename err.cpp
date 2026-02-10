#include "err.hpp"
#include <cctype>
#include <ctime>
#include <iomanip>
#include <random>
#include <sstream>

bool isValidID(const std::string &id) {
    for (const auto it : id) {
        if (!isalnum(it) && it != '_')
            return false;
    }
    return true;
}

std::string randomID() {
    std::random_device rd;
    std::string        o;
    std::mt19937_64    r(time(nullptr) + rd());
    for (int i = 0; i < 50; i++) {
        unsigned _r = r() % 26;
        if (_r < 25) {
            o.push_back((char)'a' + _r);
        } else {
            o.push_back('_');
        }
    }
    return o;
}

CrtExcept::CrtExcept(const unsigned short code) {
    std::ostringstream oss;
    oss << "0x" << std::uppercase << std::hex << std::setw(4) << std::setfill('0') << code;
    msg = oss.str() + " " + errtable.at(code);
}

CrtExcept::CrtExcept(const std::string &_msg) : msg("0x0000 Extern Error: " + _msg) {}

const char *CrtExcept::what() const noexcept {
    return msg.c_str();
}