#pragma once
#include <string>
#include <vector>

class IHack {
public:
	virtual void SetPass(std::string const& fileSrc) = 0;

protected:
	virtual bool CheckPass(const std::vector<unsigned char> & chipherText) = 0;
	virtual bool CheckHashSum(std::string const& fileSrc, std::vector<int> const& hash) = 0;
};

