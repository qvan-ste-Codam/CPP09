#include "PmergeMe.hpp"

#include <algorithm>
#include <deque>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

template class PmergeMe<std::vector<int>>;
template class PmergeMe<std::deque<int>>;

template <typename Container>
PmergeMe<Container>::PmergeMe(const PmergeMe& other)
    : mainChain(other.mainChain),
      pend(other.pend),
      stragglers(other.stragglers),
      startTime(other.startTime) {}

template <typename Container>
PmergeMe<Container>& PmergeMe<Container>::operator=(const PmergeMe& other) {
    if (this != &other) {
        mainChain = other.mainChain;
        pend = other.pend;
        stragglers = other.stragglers;
        startTime = other.startTime;
    }
    return *this;
}

int safeStoi(const std::string& input) {
    try {
        size_t pos = 0;
        int i = std::stoi(input, &pos);

        if (pos != input.length()) {
            throw std::runtime_error("not a valid int => " + input);
        }
        return i;
    } catch (const std::exception& e) {
        throw std::runtime_error("not a valid int => " + input);
    }
}

template <typename Container>
PmergeMe<Container>::PmergeMe(char** argv) {
    Container input;
    while (*argv) {
        std::string item(*argv);
        int in = safeStoi(item);
        // The subject requires numbers to be positive. As far as I can tell,
        // the code still works with negative numbers
        if (in < 0) {
            throw std::runtime_error("number is negative => " + item);
        }
        input.push_back(in);
        argv++;
    }
    if (input.size() == 1) {
        throw std::runtime_error("input of 1 item is already sorted");
    }

    std::cout << "Before: ";
    for (int i : input) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
    startTime = std::chrono::system_clock::now();
    fordJohnsonSort(input);
}

template <typename Container>
void PmergeMe<Container>::fordJohnsonSort(const Container& input) {
    if (input.size() <= 1) {
        mainChain = input;
        return;
    }

    Container inputCopy = input;
    if (input.size() % 2 != 0) {
        stragglers.push_back(inputCopy.back());
        inputCopy.pop_back();
    }

    std::vector<std::pair<int, int>> pairs;
    auto it = inputCopy.begin();
    while (it != inputCopy.end()) {
        int first = *it++;
        int second = *it++;
        if (first > second) {
            pairs.push_back(std::make_pair(first, second));
        } else {
            pairs.push_back(std::make_pair(second, first));
        }
    }

    Container largerElements;
    for (const auto& pair : pairs) {
        largerElements.push_back(pair.first);
    }

    if (largerElements.size() > 1) {
        PmergeMe<Container> sorter;
        sorter.fordJohnsonSort(largerElements);
        mainChain = sorter.mainChain;

        for (int straggler : sorter.stragglers) {
            stragglers.push_back(straggler);
        }
        for (int pendElement : sorter.pend) {
            pend.push_back(pendElement);
        }
    } else {
        mainChain = largerElements;
    }
    for (const auto& pair : pairs) {
        pend.push_back(pair.second);
    }
}

template <typename Container>
Container PmergeMe<Container>::generateJacobsthalSequence(
    std::size_t maxSize) const {
    Container jacobsthal = {0, 1};
    while (jacobsthal.back() < static_cast<int>(maxSize)) {
        jacobsthal.push_back(jacobsthal[jacobsthal.size() - 1] +
                             2 * jacobsthal[jacobsthal.size() - 2]);
    }
    return jacobsthal;
}

template <typename Container>
void PmergeMe<Container>::binaryInsert(int elementToInsert) {
    auto begin = mainChain.begin();
    auto end = mainChain.end();
    size_t count = std::distance(begin, end);

    while (count > 0) {
        size_t step = count / 2;
        auto mid = begin;
        std::advance(mid, step);

        if (*mid < elementToInsert) {
            begin = ++mid;
            count -= step + 1;
        } else {
            count = step;
        }
    }

    mainChain.insert(begin, elementToInsert);
}

template <typename Container>
Container PmergeMe<Container>::calculateInsertionOrder(
    const Container& jacobsthal, std::size_t pendSize) {
    Container insertIndices;

    if (pendSize == 0) {
        return insertIndices;
    }
    insertIndices.push_back(0);

    for (size_t i = 2;
         i < jacobsthal.size() && jacobsthal[i] <= static_cast<int>(pendSize);
         i++) {
        for (int j = jacobsthal[i] - 1; j > jacobsthal[i - 1]; j--) {
            if (j < static_cast<int>(pendSize)) {
                insertIndices.push_back(j);
            }
        }
    }

    for (size_t i = 0; i < pendSize; i++) {
        if (std::find(insertIndices.begin(), insertIndices.end(), i) ==
            insertIndices.end()) {
            insertIndices.push_back(i);
        }
    }

    return insertIndices;
}

template <typename Container>
void PmergeMe<Container>::sort() {
    Container jacobsthal = generateJacobsthalSequence(pend.size());
    Container insertIndices = calculateInsertionOrder(jacobsthal, pend.size());

    for (int idx : insertIndices) {
        auto it = pend.begin();
        std::advance(it, idx);
        int elementToInsert = *it;
        binaryInsert(elementToInsert);
    }

    for (int straggler : stragglers) {
        binaryInsert(straggler);
    }
    printResult();
}

template <typename Container>
void PmergeMe<Container>::printResult() const {
    std::chrono::time_point<Clock> endTime = std::chrono::system_clock::now();

    std::cout << "After: ";
    bool first = true;
    for (const auto& item : mainChain) {
        if (!first) std::cout << " ";
        std::cout << item;
        first = false;
    }
    std::cout << std::endl;

    std::string containerName;
    if (std::is_same<Container, std::vector<int>>::value) {
        containerName = "vector";
    } else {
        containerName = "deque";
    };

    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(
        endTime - startTime);
    std::cout << "Time to process a range of " << mainChain.size()
              << " elements with std::" << containerName << " : "
              << duration.count() << " ns" << std::endl;
}

template <typename Container>
void PmergeMe<Container>::validate() const {
    if (mainChain.empty()) {
        throw std::runtime_error("container is empty");
    }

    bool isSorted = std::is_sorted(mainChain.begin(), mainChain.end());

    if (isSorted) {
        std::cout << "Validation successful: Container is properly sorted."
                  << std::endl;
    } else {
        std::cout << "Validation failed: Container is not properly sorted!"
                  << std::endl;
    }
};
