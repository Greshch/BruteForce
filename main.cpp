#include <string>
#include <vector>
#include <fstream>
#include <exception>
#include <iostream>
#include <algorithm>
#include <memory>

#include "openssl/evp.h"
#include <openssl/aes.h>
#include "openssl/sha.h"

#include "AlgorithmsBase.h"
#include "Md_5Algorithm.h"
#include "FileWorks.h"

unsigned char key[EVP_MAX_KEY_LENGTH];
unsigned char iv[EVP_MAX_IV_LENGTH];

void PasswordToKey(std::string& password);
void EncryptAes(const std::vector<unsigned char> plainText, std::vector<unsigned char>& chipherText);
void Encrypt();
void Encrypt(const std::string& filePathDest, const std::string& filePathSrc);


void Decrypt();
void Decrypt(const std::string& filePathDest, const std::string& filePathSrc);
void DecryptAes(const std::vector<unsigned char> chipherText, std::vector<unsigned char>& decryptText);

void CalculateHash(const std::vector<unsigned char>& data, std::vector<unsigned char>& hash);

int main()
{
    std::string pass = "pass";
    std::string foldersPath = "D:/projects/cplus/Apriorit/second/BruteForce/Debug/";
    //std::string nameSimpleText = foldersPath + "plain_text";
    std::string nameEncryptedText = foldersPath + "chipher_text";
    std::string nameDecryptedText = foldersPath + "decrypted_text";
    try
    {
        PasswordToKey(pass);
        //Encrypt();
        //Encrypt(nameEncryptedText, nameSimpleText);

        //Decrypt();
        Decrypt(nameDecryptedText, nameEncryptedText);

        std::unique_ptr<AlgorithmsBase> algo(new Md_5Algorithm);
        algo.get()->PasswordToKey(pass);
    }
    catch (const std::runtime_error& ex)
    {
        std::cerr << ex.what();
    }
}

void PasswordToKey(std::string& password)
{
    OpenSSL_add_all_algorithms();
    const EVP_MD* dgst = EVP_get_digestbyname("md5");
    if (!dgst)
    {
        throw std::runtime_error("no such digest");
    }

    const unsigned char* salt = NULL;
    if (!EVP_BytesToKey(EVP_aes_128_cbc(), EVP_md5(), salt,
        reinterpret_cast<unsigned char*>(&password[0]),
        password.size(), 1, key, iv))
    {
        throw std::runtime_error("EVP_BytesToKey failed");
    }
}

void EncryptAes(const std::vector<unsigned char> plainText, std::vector<unsigned char>& chipherText)
{
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv))
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

void CalculateHash(const std::vector<unsigned char>& data, std::vector<unsigned char>& hash)
{
    std::vector<unsigned char> hashTmp(SHA256_DIGEST_LENGTH);

    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, &data[0], data.size());
    SHA256_Final(&hashTmp[0], &sha256);
    hash.swap(hashTmp);
}

void Encrypt()
{
    std::vector<unsigned char> plainText;
    std::string path = "D:/projects/cplus/Apriorit/second/BruteForce/Debug/";
    ReadFile(path + "plain_text", plainText);
    std::vector<unsigned char> hash;
    CalculateHash(plainText, hash);

    std::vector<unsigned char> chipherText;
    EncryptAes(plainText, chipherText);

    WriteFile(path + "chipher_text", chipherText);

    AppendToFile(path + "chipher_text", hash);
}

void Encrypt(const std::string& filePathDest, const std::string& filePathSrc)
{
    std::vector<unsigned char> plainText; 
    ReadFile(filePathSrc, plainText);
    std::vector<unsigned char> hash;
    CalculateHash(plainText, hash);
    std::vector<unsigned char> chipherText;
    EncryptAes(plainText, chipherText);
    WriteFile(filePathDest, chipherText);
    AppendToFile(filePathDest, hash);
}

void Decrypt()
{
    std::vector<unsigned char> chiferText;
    std::string path = "D:/projects/cplus/Apriorit/second/BruteForce/Debug/";
    ReadFile(path + "chipher_text", chiferText);

    chiferText.erase(chiferText.end() - AES_BLOCK_SIZE * 2, chiferText.end());
    std::vector<unsigned char> decryptedText;
    DecryptAes(chiferText, decryptedText);
    WriteFile(path + "decrypted_text", decryptedText);
}

void Decrypt(const std::string& filePathDest, const std::string& filePathSrc)
{
    std::vector<unsigned char> chiferText;
    ReadFile(filePathSrc, chiferText);
    chiferText.erase(chiferText.end() - AES_BLOCK_SIZE * 2, chiferText.end());
    std::vector<unsigned char> decryptedText;
    DecryptAes(chiferText, decryptedText);
    WriteFile(filePathDest, decryptedText);
}

void DecryptAes(const std::vector<unsigned char> chipherText, std::vector<unsigned char>& decryptText)
{
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();

    if (!EVP_DecryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv))
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
