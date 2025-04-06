#pragma once
#include <list>

class PmergeMe {
   public:
    PmergeMe() = default;
    PmergeMe(char **argv);
    PmergeMe(const PmergeMe &other);
    ~PmergeMe() = default;
    PmergeMe &operator=(const PmergeMe &other);

   private:
    std::list<int> input;
};
