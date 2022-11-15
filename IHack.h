#pragma once
#include <string>
#include <vector>

class IHack {
public:
	virtual bool SearchPassword(std::string const& file, std::vector<std::string> & balk) = 0;

protected:
	virtual bool CheckPass(const std::vector<unsigned char> & chipherText) = 0;
};

