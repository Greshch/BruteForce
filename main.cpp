#include "AlgorithmsBase.h"
#include "Md_5Algorithm.h"
#include "FileWorks.h"
#include <memory>
#include <exception>
#include <iostream>



int main()
{
    std::string pass = "pass";
    std::string foldersPath = "D:/projects/cplus/Apriorit/second/BruteForce/Debug/";
    //std::string nameSimpleText = foldersPath + "plain_text";
    std::string nameEncryptedText = foldersPath + "chipher_text" /*"encrypted_text"*/;
    std::string nameDecryptedText = foldersPath + "decrypted_text";
    try
    {
        std::unique_ptr<AlgorithmsBase> algo(new Md_5Algorithm);
        algo.get()->PasswordToKey(pass);
        //algo.get()->Encrypt(nameEncryptedText, nameSimpleText);
        algo.get()->Decrypt(nameDecryptedText, nameEncryptedText);
    }
    catch (const std::runtime_error& ex)
    {
        std::cerr << ex.what();
    }
}


