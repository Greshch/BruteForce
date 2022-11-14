#include "AlgorithmsBase.h"
#include "Md_5Algorithm.h"
#include "FileWorks.h"
#include <chrono>
#include <memory>
#include <exception>
#include <iostream>



int main() {
    std::string foldersPath = "D:/projects/cplus/Apriorit/second/BruteForce/";
    //std::string namePlainText = foldersPath + "plain_text";
    std::string nameEncryptedText = foldersPath + "chipher_text";
    //std::string nameDecryptedText = foldersPath + "decrypted_text";
    try {
        std::unique_ptr<AlgorithmsBase> algo(new Md_5Algorithm);
        //algo.get()->SetPassword(pass);
        //algo.get()->PasswordToKey();
        //algo.get()->Encrypt(nameEncryptedText, namePlainText);
        //algo.get()->Decrypt(nameDecryptedText, nameEncryptedText);

        auto begin = std::chrono::system_clock::now();
        time_t beginTime = std::chrono::system_clock::to_time_t(begin);
        dynamic_cast<IHack*>(algo.get())->SearchPassword(nameEncryptedText);
        auto finish = std::chrono::system_clock::now();
        time_t finishTime = std::chrono::system_clock::to_time_t(finish);
        std::cout << "time: " << finishTime - beginTime << std::endl;
    }
    catch (const std::runtime_error& ex) {
        std::cerr << ex.what();
    }
}


