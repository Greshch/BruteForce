#include "PasswordGenerator.h"
#include <iostream>
#include <cmath>
#include <algorithm>
#include "PasswordGenerator.h"

void PasswordGenerator::Update()
{
    UpdateProgresion();
    size_t len = m_progresion.m_sz;

    for (size_t i = 0; i < len; )
    {
        char cur = m_password[i];
        if (i == len - 1)
        {
            if (m_password.size() < len)
            {
                m_password.push_back(m_first);
            }
            else
            {
                auto it = std::find(m_list.begin(), m_list.end(), cur);
                m_password[i] = m_list[it - m_list.begin() + 1];
            }
            break;
        }
        else
        {
            if (m_password[i] == m_last)
            {
                m_password[i] = m_first;
                ++i;
            }
            else
            {
                auto it = std::find(m_list.begin(), m_list.end(), cur);
                m_password[i] = m_list[it - m_list.begin() + 1];
                break;
            }
        }
    }
}

size_t PasswordGenerator::GetListSize() const
{
    return m_list.size();
}

void PasswordGenerator::UpdateProgresion()
{
    if (m_index < m_progresion.m_Sum)
    {
        return;
    }
    m_progresion.m_cur *= m_progresion.m_n;
    m_progresion.m_Sum += m_progresion.m_cur;
    ++m_progresion.m_sz;
}

void PasswordGenerator::Reset()
{
    m_index = 0;
    m_amount = 0;
    m_password = "";
    m_list = "";
    m_progresion.m_cur = 0;
    m_progresion.m_n = 1;
    m_progresion.m_sz = 1;
    m_progresion.m_Sum = 0;
}

void PasswordGenerator::SetFirstAndLast()
{
    m_first = m_list[0];
    m_last = m_list[m_list.size() - 1];
}

void PasswordGenerator::SetProgresion()
{
    m_progresion.m_n = m_list.size();
    m_progresion.m_cur = 1;
    m_progresion.m_sz = 0;
    m_progresion.m_Sum = 0;
}

void PasswordGenerator::AddToVocab(char from, char to)
{
    for (char ch = from; ch <= to; ++ch)
    {
        this->m_list.push_back(ch);
    }
    SetFirstAndLast();
    SetProgresion();
}

void PasswordGenerator::AddToVocab(std::string const& list)
{
    for (auto const& ch : list)
    {
        this->m_list.push_back(ch);
    }
    SetFirstAndLast();
    SetProgresion();
}

void PasswordGenerator::SetMaxLenOfPassword(size_t len)
{
    size_t cur = 1;
    size_t n = GetListSize();
    for (size_t i = 0; i < len; i++)
    {
        cur *= n;
        m_amount += cur;
    }
}

std::string& PasswordGenerator::GetPassword()
{
    Update();
    ++this->m_index;
    return this->m_password;
}

size_t PasswordGenerator::GetAmount() const
{
    return m_amount;
}

PasswordGenerator::PasswordGenerator() : PasswordGenerator(PasswordGenerator::s_length)
{
    //ctor
}

PasswordGenerator::PasswordGenerator(size_t buffer)
{
    this->m_password.reserve(buffer);
    this->m_list.reserve(buffer * 5);
}

bool PasswordGenerator::GetPasswordBatch(std::vector<std::string>& passwords, size_t passwordsCount)
{
    int const len = GetAmount();
    if (m_index  >= len)
    {
        return false;
    }
    
    for (size_t i = m_index % passwordsCount; i < passwordsCount; ++i)
    {
        if (m_index == len)
        {
            return false;
        }
        passwords.push_back(GetPassword());
    }
    return true;
}

PasswordGenerator::~PasswordGenerator()
{
    //dtor
}
