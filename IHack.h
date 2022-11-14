#pragma once
#include <string>
#include <vector>

class IHack {
public:
	virtual bool SearchPassword(std::string const& fileSrc) = 0;
	virtual void AddToVocab(char from, char to) = 0;
	virtual void AddToVocab(std::string const& str) = 0;
	virtual void ResetVocab() = 0;
	virtual void SetMaxLenOfPassword(size_t sz) = 0;
	virtual size_t GetAmount() const = 0;

protected:
	virtual bool CheckPass(const std::vector<unsigned char> & chipherText) = 0;
};

