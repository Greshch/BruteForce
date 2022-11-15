#include "AlgorithmsBase.h"
#include "Md_5Algorithm.h"
#include "FileWorks.h"
#include <chrono>
#include <memory>
#include <exception>
#include <iostream>



int main(int argc, char** argv) {
    //std::string foldersPath = "D:/projects/cplus/Apriorit/second/BruteForce/";
    //std::string namePlainText = foldersPath + "plain_text";
    //std::string nameEncryptedText = foldersPath + "chipher_text";
    //std::string nameEncryptedText = foldersPath + "chipher_text_brute_force";
    //std::string nameDecryptedText = foldersPath + "decrypted_text";

    size_t const maxPasswordLen = 4;
    size_t const volBuffer = 32;
    try {
        if (argc == 1)
        {
            throw std::runtime_error("Lack of arguments error");
        }
        std::string nameEncryptedText = argv[1];
        std::string nameDecryptedText = "decrypted_text";

        PasswordGenerator generator;
        generator.AddToVocab('a', 'z');
        generator.AddToVocab('0', '9');
        generator.SetMaxLenOfPassword(maxPasswordLen);

        std::vector<std::string>  balk;
        
        Md_5Algorithm algo;
        bool isFound = false;
        while (!isFound)
        {
            generator.GetPasswordwordBatch(balk, volBuffer);
            isFound = algo.SearchPassword(nameEncryptedText, balk);

            balk.clear();
        }

        std::cout << "index: " << generator.GetIndex() << std::endl;
        std::cout << "key: " << algo.GetPassword() << std::endl;

        /*algo.AddToVocab('a', 'z');
        algo.AddToVocab('0', '9');
        algo.SetMaxLenOfPassword(4);
        auto begin = std::chrono::system_clock::now();
        time_t beginTime = std::chrono::system_clock::to_time_t(begin);

        bool isFound = algo.SearchPassword(nameEncryptedText);

        auto finish = std::chrono::system_clock::now();
        time_t finishTime = std::chrono::system_clock::to_time_t(finish);
        std::cout << "time: " << finishTime - beginTime << std::endl;
        if (isFound)
        {
            algo.Decrypt(nameDecryptedText, nameEncryptedText);
            std::cout << "index: " << algo.GetIndex() << std::endl;
            std::cout << "key: " << algo.GetPassword() << std::endl;
        }*/
    }
    catch (const std::runtime_error& ex) {
        std::cerr << ex.what();
    }
}


