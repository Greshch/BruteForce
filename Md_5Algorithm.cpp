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

void Md_5Algorithm::PasswordToKey() {
    PasswordToKey(m_pass);
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
    chiferText.erase(chiferText.end() - SHA256_DIGEST_LENGTH, chiferText.end());
    std::vector<unsigned char> decryptedText;
    DecryptAes(chiferText, decryptedText);
    WriteFile(filePathDest, decryptedText);
}

void Md_5Algorithm::PrepearForHack(std::string const& file)
{
    ReadFile(file, m_chiferText);
    GetHash(m_originHash, m_chiferText);
    m_chiferText.erase(m_chiferText.end() - SHA256_DIGEST_LENGTH, m_chiferText.end());
}

bool Md_5Algorithm::SearchPassword(std::string const& file, std::vector<std::string>& balk)
{
    for (auto& key : balk)
    {
        PasswordToKey(key);
        if (CheckPass(m_chiferText))
        {
            DecryptAes(m_chiferText, m_decryptedText);
            CalculateHash(m_decryptedText, m_curHash);
            if (m_originHash == m_curHash)
            {
                SetPassword(key);
                return true;
            }
        }
    }
    return false;
}

void Md_5Algorithm::CalculateHash(const std::vector<unsigned char>& data, std::vector<unsigned char>& hash) {
    std::vector<unsigned char> hashTmp(SHA256_DIGEST_LENGTH);
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, &data[0], data.size());
    SHA256_Final(&hashTmp[0], &sha256);
    hash.swap(hashTmp);
}

void Md_5Algorithm::EncryptAes(const std::vector<unsigned char> & plainText, std::vector<unsigned char>& chipherText) {
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

void Md_5Algorithm::DecryptAes(const std::vector<unsigned char> & chipherText, std::vector<unsigned char>& decryptText) {
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();

    if (!EVP_DecryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, m_key, m_iv)) {
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

void Md_5Algorithm::PasswordToKey(std::string& pass) {
    const unsigned char* salt = NULL;
    if (!EVP_BytesToKey(EVP_aes_128_cbc(), EVP_md5(), salt,
        reinterpret_cast<unsigned char*>(&pass[0]),
        pass.size(), 1, m_key, m_iv)) {
        throw std::runtime_error("EVP_BytesToKey failed");
    }
}

bool Md_5Algorithm::CheckPass(const std::vector<unsigned char> & chipherText) {
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();

    if (!EVP_DecryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, m_key, m_iv)) {
        return false;
    }
    std::vector<unsigned char> decryptTextBuf(chipherText.size());
    int decryptTextSize = 0;
    if (!EVP_DecryptUpdate(ctx, &decryptTextBuf[0], &decryptTextSize, &chipherText[0], chipherText.size())) {
        EVP_CIPHER_CTX_free(ctx);
        return false;
    }
    int lastPartLen = 0;
    if (!EVP_DecryptFinal_ex(ctx, &decryptTextBuf[0] + decryptTextSize, &lastPartLen)) {
        EVP_CIPHER_CTX_free(ctx);
        return false;
    }
    decryptTextBuf.erase(decryptTextBuf.begin() + decryptTextSize + lastPartLen, decryptTextBuf.end());
    EVP_CIPHER_CTX_free(ctx);
    return true;
}

void Md_5Algorithm::GetHash(std::vector<unsigned char>& dest, std::vector<unsigned char> const& src) const
{
    dest.resize(SHA256_DIGEST_LENGTH);
    std::copy(src.end() - SHA256_DIGEST_LENGTH, src.end(), dest.begin());
}

