#include "AlgorithmsBase.h"
#include "Md_5Algorithm.h"
#include "FileWorks.h"
#include <memory>
#include <exception>
#include <iostream>



int main() {
    std::string pass = "5";
    std::string foldersPath = "D:/projects/cplus/Apriorit/second/BruteForce/Debug/";
    std::string namePlainText = foldersPath + "plain_text";
    std::string nameEncryptedText = foldersPath + "chipher_text";
    std::string nameDecryptedText = foldersPath + "decrypted_text";
    try {
        std::unique_ptr<AlgorithmsBase> algo(new Md_5Algorithm);
        algo.get()->SetPassword(pass);
        algo.get()->PasswordToKey();
        //algo.get()->Encrypt(nameEncryptedText, namePlainText);
        //algo.get()->Decrypt(nameDecryptedText, nameEncryptedText);
        dynamic_cast<IHack*>(algo.get())->SetPass(nameEncryptedText);
    }
    catch (const std::runtime_error& ex) {
        std::cerr << ex.what();
    }
}


