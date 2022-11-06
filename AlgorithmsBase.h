#pragma once
#include "openssl/evp.h"
#include <string>
#include <vector>
#include <array>

class AlgorithmsBase {
public:
	virtual void PasswordToKey(std::string& password) = 0;

	AlgorithmsBase() = default;

	std::array<unsigned char, EVP_MAX_KEY_LENGTH> GetKey() const;
	void SetKey(unsigned char[EVP_MAX_KEY_LENGTH], size_t sz = EVP_MAX_KEY_LENGTH);

	std::array<unsigned char, EVP_MAX_IV_LENGTH> GeIV() const;
	void SetIV(unsigned char[EVP_MAX_IV_LENGTH], size_t sz = EVP_MAX_IV_LENGTH);

	std::string const& GetPass() const;
	void SetPass(std::string const& pass);

	virtual ~AlgorithmsBase() = default;

private:
	unsigned char m_key[EVP_MAX_KEY_LENGTH] {};
	unsigned char m_iv[EVP_MAX_IV_LENGTH] {};
	std::string m_pass {};
};

