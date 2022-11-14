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
        Md_5Algorithm algo;
        algo.AddToVocab('a', 'z');
        algo.SetMaxLenOfPassword(4);
        auto begin = std::chrono::system_clock::now();
        time_t beginTime = std::chrono::system_clock::to_time_t(begin);

        algo.SearchPassword(nameEncryptedText);

        auto finish = std::chrono::system_clock::now();
        time_t finishTime = std::chrono::system_clock::to_time_t(finish);
        std::cout << "time: " << finishTime - beginTime << std::endl;
    }
    catch (const std::runtime_error& ex) {
        std::cerr << ex.what();
    }
}


