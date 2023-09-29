#pragma once

#include <iostream>
#include <type_traits>

template <typename T, std::size_t Size = 50>
struct Stack {
    constexpr static auto npos = -1;

    T stack[Size];
    int top = -1;

    bool empty() {
        return top == -1;
    }

    bool has_two() {
        return top >= 1;
    }

    bool full() {
        return top == Size - 1;
    }

    void push(const T& elem) {
        if (this->full()) return;
        stack[++top] = elem;
    }

    T pop() {
        if (empty()) return T();
        return stack[top--];
    }

    void clear() {
        top = -1;
    }
};