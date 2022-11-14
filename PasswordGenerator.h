#ifndef PasswordGenerator_H
#define PasswordGenerator_H

#include <string>
#include <vector>


class PasswordGenerator
{
    public:
    PasswordGenerator();
    PasswordGenerator(size_t buffer);
    bool GetPasswordBatch(std::vector<std::string>& passwords, size_t passwordsCount);
    std::string& GetPassword();
    size_t GetAmount() const;
    void SetList(char from, char to);
    void SetList(std::string const& list);
    void SetMaxLenOfPassword(size_t len);
    size_t GetListSize() const;
    virtual ~PasswordGenerator();

    protected:
    static const size_t s_length = 32;
    void Update();
    void SetFirstAndLast();
    void SetProgresion();
    void UpdateProgresion();
    void Reset();

    private:
    std::string m_password;
    std::string m_list;
    size_t m_index = 0;
    size_t m_amount = 0;
    char m_first;
    char m_last;
    struct Progresion {
        size_t m_n = 0;
        size_t m_cur = 1;
        size_t m_sz = 1;
        size_t m_Sum = 0;
    } m_progresion;
};

#endif // PasswordGenerator_H
