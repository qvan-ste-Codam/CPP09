#include "RPN.hpp"

#include <iostream>
#include <sstream>
#include <stdexcept>

RPN::RPN(const std::string& input) : stack() {
    if (input[0] < '0' || input[0] > '9') {
        throw std::runtime_error("Sequence has to start with a number");
    };
    std::stringstream ss(input);
    std::string line;

    while (std::getline(ss, line, ' ')) {
        if (line.length() > 1) {
            throw std::runtime_error("Invalid character sequence");
        }
        char c = line[0];
        if (c >= '0' && c <= '9') {
            stack.push(c - '0');
        } else {
            handleOperation(c);
        }
    }
    if (stack.size() != 1) {
        throw std::runtime_error("Invalid operand sequence");
    }
    std::cout << stack.top() << std::endl;
}

RPN::RPN(const RPN& other) : stack(other.stack) {}

RPN& ::RPN::operator=(const RPN & other) {
    if (this != &other) {
        stack = other.stack;
    }
    return *this;
}

void RPN::handleOperation(char c) {
    if (stack.size() < 2) {
        throw std::runtime_error(
            "Stack doesn't contain 2 items to do operation");
    }
    int a = stack.top();
    stack.pop();
    int b = stack.top();
    stack.pop();

    switch (c) {
        case '+':
            stack.push(b + a);
            break;
        case '-':
            stack.push(b - a);
            break;
        case '*':
            stack.push(b * a);
            break;
        case '/':
            if (a == 0) {
                throw std::runtime_error("Trying to divide by zero");
            }
            stack.push(b / a);
            break;
        default:
            throw std::runtime_error(std::string("Invalid char: ") + c);
    }
}
