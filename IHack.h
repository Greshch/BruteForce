#pragma once
#include <string>
#include <vector>

class IHack {
public:
	virtual std::string SetPass() const = 0;

protected:
	virtual bool CheckPass(std::string const& pass) = 0;
	virtual bool CheckHashSum(std::vector<int> const& hash) = 0;
};

