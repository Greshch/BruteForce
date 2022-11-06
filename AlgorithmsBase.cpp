#include "AlgorithmsBase.h"

#include <string>
#include <vector>
#include <fstream>
#include <exception>
#include <iostream>
#include <algorithm>
#include <array>

#include "openssl/evp.h"
#include <openssl/aes.h>
#include "openssl/sha.h"

std::array<unsigned char, EVP_MAX_KEY_LENGTH> AlgorithmsBase::GetKey() const {
	std::array<unsigned char, EVP_MAX_KEY_LENGTH> tmp;
	std::copy(std::begin(m_key), std::end(m_key), tmp.begin());
	return tmp;
}

void AlgorithmsBase::SetKey(unsigned char key[EVP_MAX_KEY_LENGTH], size_t sz) {
	std::copy(key, key + EVP_MAX_KEY_LENGTH, m_key);
}

std::array<unsigned char, EVP_MAX_IV_LENGTH> AlgorithmsBase::GeIV() const {
	std::array<unsigned char, EVP_MAX_IV_LENGTH> tmp;
	std::copy(std::begin(m_iv), std::end(m_iv), tmp.begin());
	return tmp;
}

void AlgorithmsBase::SetIV(unsigned char iv[EVP_MAX_IV_LENGTH], size_t sz) {
	std::copy(iv, iv + EVP_MAX_IV_LENGTH, m_iv);
}

std::string const& AlgorithmsBase::GetPass() const {
	return m_pass;
}

void AlgorithmsBase::SetPass(std::string const& pass) {
	m_pass = pass;
}
