#include "err.hpp"
#include <cctype>
#include <ctime>
#include <iomanip>
#include <random>
#include <sstream>

bool isInvalidID(const std::string &id) {
    for (const auto it : id) {
        if (!isalnum(it) && it != '_')
            return true;
    }
    return false;
}

std::string randomID(unsigned len) {
    std::random_device rd;
    std::string        o;
    std::mt19937_64    r(time(nullptr) + rd());
    for (unsigned i = 0; i < len; i++) {
        uint8_t _r = r() % 63;
        if (_r < 26)
            o.push_back((char)'a' + _r); // 0 ~ 25 : 'a' ~ 'z'
        else if (_r < 52)
            o.push_back((char)'A' + _r - 26); // 26 ~ 51 : 'A' ~ 'Z'
        else if (_r < 62)
            o.push_back((char)'0' + _r - 52); // 52 ~ 61 : '0' ~ '9'
        else
            o.push_back('_'); // 62 : '_'
    }
    return o;
}

const std::string &CrtExcept::how() const noexcept { return detail; }

const char *CrtExcept::what() const noexcept { return _(errmsgs[code]); }

uint16_t CrtExcept::which() const noexcept { return code; }

const std::string CrtExcept::whichStr() const {
    std::ostringstream oss;
    oss << "0x" << std::uppercase << std::hex << std::setw(4) << std::setfill('0') << code;
    return oss.str();
}
