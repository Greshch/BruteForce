#include <Windows.h>
#include "AlgorithmsBase.h"
#include "Md_5Algorithm.h"
#include "FileWorks.h"
#include "TimeWorks.h"
#include <memory>
#include <exception>
#include <iostream>
#include <cstring>
#include <condition_variable>

void Batcher(std::condition_variable& cv, 
    PasswordGenerator& generator, 
    std::vector<std::string>&  balk,
    size_t const volBuffer,
    std::atomic_bool& generated,
    std::atomic_bool& found
)
{
    while (!found)
    {
        generator.GetPasswordwordBatch(balk, volBuffer);
        generated = true;
        cv.notify_all();
    }
}

void Search(
    std::condition_variable& cv, 
    std::mutex& mtx, 
    Md_5Algorithm& algo,
    std::vector<std::string>& balk, 
    size_t const from, 
    size_t const to,
    std::atomic_bool& generated,
    std::atomic_bool& found
)
{
    /*static int cnt = 0;
    std::ofstream log;
    std::string msg;
    while (true)
    {
        std::unique_lock<std::mutex> uniLock(mtx);
        cv.wait(uniLock, [&flag]() { return flag; });

        log.open("log.log", std::ios_base::app);
        msg = "Logger: " + std::to_string(++cnt) + "\n";
        log.write(msg.c_str(), msg.length());

        log.close();

        std::this_thread::sleep_for(std::chrono::milliseconds(300));
    }*/

    std::vector<std::string> semiBalk(1024);

    while (true)
    {
        std::unique_lock<std::mutex> uniLock(mtx);
        cv.wait(uniLock, [&generated]() { return generated == true; });

        semiBalk.clear();
        std::copy(balk.begin() + from, balk.begin() + to, semiBalk.begin());
        balk.clear();
        found = algo.SearchPassword(balk);
        if (found)
        {
            break;
        }
    }
}



int main(int argc, char** argv) {
    //std::string foldersPath = "D:/projects/cplus/Apriorit/second/BruteForce/";
    //std::string namePlainText = foldersPath + "plain_text";
    //std::string nameEncryptedText = foldersPath + "chipher_text";
    //std::string nameEncryptedText = foldersPath + "chipher_text_brute_force";
    //std::string nameDecryptedText = foldersPath + "decrypted_text";

    size_t const maxPasswordLen = 4;
    char const* log = "--log_passwords";
    bool isLog = false;
    size_t const volBuffer = 1024;
    try {
        if (argc == 1)
        {
            throw std::runtime_error("Lack of arguments error");
        }
        else if (argc == 3)
        {
            if (strcmp(argv[2], log) != 0)
            {
                throw std::runtime_error("Wrong argument error");
            }
            isLog = true;
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
       
       /* 
        bool isFound = false;
        auto beginTime = timeSinceEpochMillisec();
        time_t const updateTime = 100;
        bool flag = true;
        while (!isFound)
        {
            generator.GetPasswordwordBatch(balk, volBuffer);
            if (isLog)
            {
                std::string logFileName = "log.pass";
                AppendToFile(logFileName, balk);
            }
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
        */

        std::condition_variable cv;
        std::mutex mtx;
        std::atomic_bool isFound = false;
        std::atomic_bool generated = false;

        /*std::thread th1(
            Batcher, 
            std::ref(cv),
            std::ref(generator),
            std::ref(balk),
            volBuffer,
            std::ref(generated),
            std::ref(isFound)
        );

        std::thread th2(
            Search,
            std::ref(cv),
            std::ref(mtx),
            std::ref(algo),
            std::ref(balk),
            0,
            volBuffer,
            std::ref(generated),
            std::ref(isFound)
        );

        th1.join();
        th2.join();*/

        auto start = timeSinceEpochMillisec();

        std::thread batcher([&]()
            {
                while (!isFound && generator.GetIndex() < generator.GetAmount())
                {
                    balk.clear();
                    generator.GetPasswordwordBatch(balk, volBuffer);
                    generated = true;
                    cv.notify_all();
                    std::this_thread::sleep_for(std::chrono::milliseconds(8));
                }
            }
        );
        
        std::thread searcher([&]()
            {
                while (true)
                {
                    std::unique_lock<std::mutex> uniLock(mtx);
                    cv.wait(uniLock, [&generated]() { return generated == true; });
                    isFound = algo.SearchPassword(balk);
                    if (isFound)
                    {
                        break;
                    }
                }
            }
        );

        batcher.join();
        searcher.detach();

        if (isFound)
        {
            auto period = timeSinceEpochMillisec() - start;
            std::cout << "Time elapsed: " << period << " milisec" << std::endl;
        }
    }
    catch (const std::runtime_error& ex) {
        std::cerr << ex.what();
    }
}


