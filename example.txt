#include <iostream>
#include "PasswordGenerator.h"
/*
* Пример: добавим все числа от 0 до 9 и выведем все пароли размером от 1 символа до 3-х включительно
*/
using namespace std;

int main()
{
    PasswordGenerator bruteForceAttack;
    bruteForceAttack.SetList('0', '9');
    //bruteForceAttack.SetList('a', 'z');
    //bruteForceAttack.SetList('A', 'Z');
    //bruteForceAttack.SetList("!@#$%^&*()-_+={}[]?");
    bruteForceAttack.SetMaxLenOfPassword(4);
    int const len = bruteForceAttack.GetAmount();
    std::vector<std::string> buffer;
    bool next = true;
    while (next)
    {
        next = bruteForceAttack.GetPasswordBatch(buffer, 16);
        for (auto const& e : buffer)
        {
            std::cout << e << " ";
        }
        std::cout << std::endl;
        buffer.clear();
    }

    return 0;
}
