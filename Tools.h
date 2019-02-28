#pragma once
#include <iostream>
#include <cstdlib>
#include <cstring>

class Tools {
public:
	int toInt(const std::string&);
	std::string strToCapLet(const std::string&);
	bool toValidEmail(const std::string&);

};