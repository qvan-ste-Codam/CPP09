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

   private:
    Container stragglers;
    Container mainChain;
    std::vector<std::pair<Container, Container>> pairs;
    std::chrono::time_point<Clock> startTime;

    void createInitialPairs(const Container& input);
    void displayResult();
    void printPairs() const;
    void mergePairs();
    void printList(const Container& list) const;
    void printResult() const;
    Container generateJacobsthalSequence(std::size_t maxSize) const;
    void binaryInsert(int elementToInsert);
    Container calculateInsertionOrder(const Container& jacobsthal,
                                      std::size_t pendSize);
};
