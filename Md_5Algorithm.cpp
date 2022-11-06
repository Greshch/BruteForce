#include "Md_5Algorithm.h"
#include <exception>
#include <iostream>

void Md_5Algorithm::PasswordToKey(std::string& password) {
    OpenSSL_add_all_algorithms();
    const EVP_MD* dgst = EVP_get_digestbyname("md5");
    if (!dgst)
    {
        throw std::runtime_error("no such digest");
    }

    const unsigned char* salt = NULL;
    if (!EVP_BytesToKey(EVP_aes_128_cbc(), EVP_md5(), salt,
        reinterpret_cast<unsigned char*>(&password[0]),
        password.size(), 1, m_key, m_iv))
    {
        throw std::runtime_error("EVP_BytesToKey failed");
    }
}

void Md_5Algorithm::Encrypt(const std::string& filePathDest, const std::string& filePathSrc) {
}

void Md_5Algorithm::Decrypt(const std::string& filePathDest, const std::string& filePathSrc) {
}

void Md_5Algorithm::CalculateHash(const std::vector<unsigned char>& data, std::vector<unsigned char>& hash) {
}
