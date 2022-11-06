#pragma once
#include "AlgorithmsBase.h"
#include "openssl/sha.h"

class Md_5Algorithm : public AlgorithmsBase {
public:
	void PasswordToKey(std::string& password) override;
	void Encrypt(const std::string& filePathDest, const std::string& filePathSrc) override;
	void Decrypt(const std::string& filePathDest, const std::string& filePathSrc) override;

protected:
	void CalculateHash(const std::vector<unsigned char>& data, std::vector<unsigned char>& hash) override;
};

