#pragma once
#include "openssl/evp.h"
#include <string>
#include <vector>
#include <array>

class AlgorithmsBase {
public:
	virtual void PasswordToKey() = 0;
	virtual void Encrypt(const std::string& filePathDest, const std::string& filePathSrc) = 0;
	virtual void Decrypt(const std::string& filePathDest, const std::string& filePathSrc) = 0;

	void SetPass(std::string const& pass);
	std::string const& GetPass() const;

	AlgorithmsBase();
	~AlgorithmsBase() = default;

protected:
	unsigned char m_key[EVP_MAX_KEY_LENGTH] {};
	unsigned char m_iv[EVP_MAX_IV_LENGTH] {};
	std::string m_pass;

protected:
	virtual void CalculateHash(const std::vector<unsigned char>& data, std::vector<unsigned char>& hash) = 0;
	virtual void EncryptAes(const std::vector<unsigned char> plainText, std::vector<unsigned char>& chipherText) = 0;
	virtual void DecryptAes(const std::vector<unsigned char> chipherText, std::vector<unsigned char>& decryptText) = 0;
};

