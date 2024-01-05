//
// Created by Bram Nijenkamp on 05-01-2024.
//

#pragma once
#include <memory>

template<typename T>
class Temporary {
   public:
    Temporary(T& variable, T value) : m_variable(variable), m_old_value(variable) {
        m_variable = value;
    }
    ~Temporary() {
        m_variable = m_old_value;
    }
    Temporary(Temporary&&) = delete;
    Temporary(const Temporary&) = delete;

   private:
    T& m_variable;
    T m_old_value;
};
