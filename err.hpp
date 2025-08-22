#pragma once

#include <cctype>
#include <stdexcept>
#include <unordered_map>
#include <string>

std::unordered_map<unsigned short, std::string> errtable {
	{0x0001, "0x0001 Empty ID"},
	{0x0002, "0x0002 The ID includes invalid character(s)"},
	{0x0003, "0x0003 Invalid color code; type `help --color-code` for more info"},
	{0x0004, "0x0004 No such block in this kit"},
	{0x0005, "0x0005 Invalid position"}
};
class errcode : public std::runtime_error {
public:
	errcode(const unsigned short code) : std::runtime_error(errtable.at(code)) {}
	errcode(const std::string& msg) : std::runtime_error("0x0000 Extern Error: " + msg) {}
};

bool isValidID(const std::string& id) {
	for (const auto it : id) {
		if (!isalnum(it) && it != '_') return false;
	}
	return true;
}
