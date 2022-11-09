#include "Md_5Algorithm.h"
#include "FileWorks.h"
#include <openssl/aes.h>
#include <exception>
#include <iostream>

Md_5Algorithm::Md_5Algorithm() : m_dgst(EVP_get_digestbyname("md5")) {
    if (!m_dgst) {
        throw std::runtime_error("no such digest");
    }
}

void Md_5Algorithm::PasswordToKey(std::string& password) {
    const unsigned char* salt = NULL;
    if (!EVP_BytesToKey(EVP_aes_128_cbc(), EVP_md5(), salt,
        reinterpret_cast<unsigned char*>(&password[0]),
        password.size(), 1, m_key, m_iv)) {
        throw std::runtime_error("EVP_BytesToKey failed");
    }
}

void Md_5Algorithm::Encrypt(const std::string& filePathDest, const std::string& filePathSrc) {
    std::vector<unsigned char> plainText;
    ReadFile(filePathSrc, plainText);
    std::vector<unsigned char> hash;
    CalculateHash(plainText, hash);
    std::vector<unsigned char> chipherText;
    EncryptAes(plainText, chipherText);
    WriteFile(filePathDest, chipherText);
    AppendToFile(filePathDest, hash);
}

void Md_5Algorithm::Decrypt(const std::string& filePathDest, const std::string& filePathSrc) {
    std::vector<unsigned char> chiferText;
    ReadFile(filePathSrc, chiferText);
    chiferText.erase(chiferText.end() - AES_BLOCK_SIZE * 2, chiferText.end());
    std::vector<unsigned char> decryptedText;
    DecryptAes(chiferText, decryptedText);
    WriteFile(filePathDest, decryptedText);
}

void Md_5Algorithm::CalculateHash(const std::vector<unsigned char>& data, std::vector<unsigned char>& hash) {
    std::vector<unsigned char> hashTmp(SHA256_DIGEST_LENGTH);
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, &data[0], data.size());
    SHA256_Final(&hashTmp[0], &sha256);
    hash.swap(hashTmp);
}

void Md_5Algorithm::EncryptAes(const std::vector<unsigned char> plainText, std::vector<unsigned char>& chipherText) {
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, m_key, m_iv))
    {
        throw std::runtime_error("EncryptInit error");
    }

    std::vector<unsigned char> chipherTextBuf(plainText.size() + AES_BLOCK_SIZE);
    int chipherTextSize = 0;
    if (!EVP_EncryptUpdate(ctx, &chipherTextBuf[0], &chipherTextSize, &plainText[0], plainText.size())) {
        EVP_CIPHER_CTX_free(ctx);
        throw std::runtime_error("Encrypt error");
    }

    int lastPartLen = 0;
    if (!EVP_EncryptFinal_ex(ctx, &chipherTextBuf[0] + chipherTextSize, &lastPartLen)) {
        EVP_CIPHER_CTX_free(ctx);
        throw std::runtime_error("EncryptFinal error");
    }
    chipherTextSize += lastPartLen;
    chipherTextBuf.erase(chipherTextBuf.begin() + chipherTextSize, chipherTextBuf.end());

    chipherText.swap(chipherTextBuf);
    EVP_CIPHER_CTX_free(ctx);
}

void Md_5Algorithm::DecryptAes(const std::vector<unsigned char> chipherText, std::vector<unsigned char>& decryptText) {
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();

    if (!EVP_DecryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, m_key, m_iv))
    {
        throw std::runtime_error("DecryptInit error");
    }
    std::vector<unsigned char> decryptTextBuf(chipherText.size());
    int decryptTextSize = 0;
    if (!EVP_DecryptUpdate(ctx, &decryptTextBuf[0], &decryptTextSize, &chipherText[0], chipherText.size())) {
        EVP_CIPHER_CTX_free(ctx);
        throw std::runtime_error("Decrypt error");
    }
    int lastPartLen = 0;
    if (!EVP_DecryptFinal_ex(ctx, &decryptTextBuf[0] + decryptTextSize, &lastPartLen)) {
        EVP_CIPHER_CTX_free(ctx);
        throw std::runtime_error("DecryptFinal error");
    }
    decryptTextBuf.erase(decryptTextBuf.begin() + decryptTextSize + lastPartLen, decryptTextBuf.end());
    decryptText.swap(decryptTextBuf);
    EVP_CIPHER_CTX_free(ctx);
}
