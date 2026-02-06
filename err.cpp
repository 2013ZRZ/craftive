#include <cctype>
#include <ctime>
#include <random>

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
        unsigned r_ = r() % 26;
        if (r_ < 25) {
            o.push_back((char)'a' + r_);
        } else {
            o.push_back('_');
        }
    }
    return o;
}
