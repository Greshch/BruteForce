#include "AlgorithmsBase.h"
#include "Md_5Algorithm.h"
#include "FileWorks.h"
#include <chrono>
#include <memory>
#include <exception>
#include <iostream>

uint64_t timeSinceEpochMillisec() {
    using namespace std::chrono;
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}



int main(int argc, char** argv) {
    //std::string foldersPath = "D:/projects/cplus/Apriorit/second/BruteForce/";
    //std::string namePlainText = foldersPath + "plain_text";
    //std::string nameEncryptedText = foldersPath + "chipher_text";
    //std::string nameEncryptedText = foldersPath + "chipher_text_brute_force";
    //std::string nameDecryptedText = foldersPath + "decrypted_text";

    size_t const maxPasswordLen = 4;
    size_t const volBuffer = 1024;
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
        algo.PrepearForHack(nameEncryptedText);
        bool isFound = false;
        auto beginTime = timeSinceEpochMillisec();
        time_t const updateTime = 100;
        bool flag = true;
        while (!isFound)
        {
            generator.GetPasswordwordBatch(balk, volBuffer);
            isFound = algo.SearchPassword(balk);
            auto curTime = timeSinceEpochMillisec();
            auto spendTimeTillBegin = curTime - beginTime;

            if (flag && spendTimeTillBegin && spendTimeTillBegin % updateTime == 0) {
                double progress = static_cast<double>(generator.GetIndex()) / generator.GetAmount() * 100.0;
                std::cout << generator.GetIndex() << " from " << generator.GetAmount() << " passwords " <<
                    " [" << std::round(progress) << "%]" << std::endl;
                flag = false;
            }
            else if (spendTimeTillBegin % updateTime) {
                flag = true;
            }

           
            balk.clear();
        }
        auto finishTime = timeSinceEpochMillisec();
        auto period = finishTime - beginTime;
        auto speed = 1.0 * generator.GetIndex() / period;
        std::cout << "Time elapsed: " << period << " milisec" << std::endl;
        std::cout << "Speed: " << std::round(speed) << " pass/milisec" << std::endl;
        std::cout << "key: " << algo.GetPassword() << std::endl;
        algo.Decrypt(nameDecryptedText, nameEncryptedText);
    }
    catch (const std::runtime_error& ex) {
        std::cerr << ex.what();
    }
}


