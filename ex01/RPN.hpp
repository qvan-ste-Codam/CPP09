#pragma once

#include <stack>
#include <string>
class RPN {
   public:
    RPN() = default;
    RPN(const std::string &input);
    RPN(const RPN &other);
    ~RPN() = default;
    RPN &operator=(const RPN &other);

   private:
    std::stack<int> stack;

    void handleOperation(char c);
};
