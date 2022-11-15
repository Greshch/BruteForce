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

void AlgorithmsBase::SetPassword(std::string const& pass) {
	m_pass = pass;
}

std::string const& AlgorithmsBase::GetPassword() const {
	return m_pass;
}

AlgorithmsBase::AlgorithmsBase() {
	OpenSSL_add_all_algorithms();
}
