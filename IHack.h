#pragma once
#include <string>
#include <vector>

class IHack {
public:
	virtual void SetPass(std::string const& fileSrc) = 0;

protected:
	virtual bool CheckPass(std::string const& fileSrc, std::string const& pass) = 0;
	virtual bool CheckHashSum(std::string const& fileSrc, std::vector<int> const& hash) = 0;
};

