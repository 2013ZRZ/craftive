#pragma once

#include <cctype>
#include <ctime>
#include <iomanip>
#include <random>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>

/*std::string hex2str(const int hex) {
	std::stringstream ss;
	ss << std::hex << hex;
	return ss.str();
}*/

const std::unordered_map<unsigned short, std::string> errtable {
	{0x0001, "0x0001 Empty ID"},
	{0x0002, "0x0002 The ID includes invalid character(s)"},
	{0x0003, "0x0003 Invalid color code; type `help --color-code` for more info"},
	{0x0004, "0x0004 No such block or combination in this kit"},
	{0x0005, "0x0005 Invalid position"},
	{0x0006, "0x0006 Failed to parse JSON"}
};
class errcode : public std::runtime_error {
public:
	errcode(const unsigned short code) : std::runtime_error(/*hex2str(code) + " " + */errtable.at(code)) {}
	errcode(const std::string& msg) : std::runtime_error("0x0000 Extern Error: " + msg) {}
};

bool isValidID(const std::string& id) {
	for (const auto it : id) {
		if (!isalnum(it) && it != '_') return false;
	}
	return true;
}

std::string randomID() {
	std::string o;
	std::mt19937_64 r(time(nullptr));
	for (int i = 0; i < 50; i++) {
		unsigned r_ = r() % 26;
		if (r_ < 25) o.push_back((char)'a' + r_);
		else o.push_back('_');
	}
	return o;
}
