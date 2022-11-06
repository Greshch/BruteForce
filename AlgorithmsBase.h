#pragma once
#include "openssl/evp.h"
#include <string>
#include <vector>
#include <array>

class AlgorithmsBase {
public:
	virtual void PasswordToKey(std::string& password) = 0;
	virtual void Encrypt(const std::string& filePathDest, const std::string& filePathSrc) = 0;
	virtual void Decrypt(const std::string& filePathDest, const std::string& filePathSrc) = 0;

	AlgorithmsBase() = default;
	~AlgorithmsBase() = default;

protected:
	unsigned char m_key[EVP_MAX_KEY_LENGTH] {};
	unsigned char m_iv[EVP_MAX_IV_LENGTH] {};

protected:
	virtual void CalculateHash(const std::vector<unsigned char>& data, std::vector<unsigned char>& hash) = 0;
};

