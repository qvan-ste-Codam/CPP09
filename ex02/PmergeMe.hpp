#pragma once
#include <chrono>
#include <vector>

template <typename Container>

class PmergeMe {
    typedef std::chrono::system_clock Clock;

   public:
    PmergeMe() = default;
    PmergeMe(char** argv);
    PmergeMe(const PmergeMe& other);
    ~PmergeMe() = default;
    PmergeMe& operator=(const PmergeMe& other);

    void sort();
    void validate() const;

   private:
    Container mainChain;
    Container pend;
    std::vector<std::pair<Container, Container>> pairs;
    Container stragglers;

    std::chrono::time_point<Clock> startTime;

    void fordJohnsonSort(const Container& input);
    void printResult() const;
    Container generateJacobsthalSequence(std::size_t maxSize) const;
    void binaryInsert(int elementToInsert);
    Container calculateInsertionOrder(const Container& jacobsthal,
                                      std::size_t pendSize);
};
