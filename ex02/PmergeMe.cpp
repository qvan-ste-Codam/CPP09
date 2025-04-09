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
    : stragglers(other.stragglers), pairs(other.pairs) {}

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
        input.push_back(safeStoi(item));
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
    createInitialPairs(input);
}

template <typename Container>
void PmergeMe<Container>::createInitialPairs(const Container& input) {
    auto it = input.begin();
    bool hasStraggler = (input.size() % 2 != 0);

    while (it != input.end() && std::next(it) != input.end()) {
        int first = *it++;
        int second = *it++;

        Container larger, smaller;
        if (first > second) {
            larger.push_back(first);
            smaller.push_back(second);
        } else {
            larger.push_back(second);
            smaller.push_back(first);
        }

        pairs.push_back(std::make_pair(smaller, larger));
    }

    if (hasStraggler && it != input.end()) {
        stragglers.push_back(*it);
    }

    // std::cout << "Initial pairs: ";
    // printPairs();
    // std::cout << std::endl;
}

template <typename Container>
void PmergeMe<Container>::mergePairs() {
    if (pairs.size() <= 1) {
        return;
    }
    std::vector<std::pair<Container, Container>> newPairs;

    for (auto& pair : pairs) {
        auto& [left, right] = pair;

        if (!left.empty() && !right.empty() && left.back() > right.back()) {
            std::swap(left, right);
        }
    }

    auto it = pairs.begin();
    while (it != pairs.end()) {
        auto [left1, left2] = *it++;

        if (it == pairs.end()) {
            stragglers.insert(stragglers.end(), left1.begin(), left1.end());
            stragglers.insert(stragglers.end(), left2.begin(), left2.end());
            break;
        }
        auto [right1, right2] = *it++;

        Container newLeft;
        Container newRight;
        newLeft.insert(newLeft.end(), left1.begin(), left1.end());
        newLeft.insert(newLeft.end(), left2.begin(), left2.end());
        newRight.insert(newRight.end(), right1.begin(), right1.end());
        newRight.insert(newRight.end(), right2.begin(), right2.end());
        newPairs.push_back(std::make_pair(newLeft, newRight));
    }
    pairs = newPairs;
    // std::cout << "After merge: ";
    // printPairs();
    // std::cout << std::endl;
    mergePairs();
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
         ++i) {
        for (int j = jacobsthal[i] - 1; j > jacobsthal[i - 1]; --j) {
            if (j < static_cast<int>(pendSize)) {
                insertIndices.push_back(j);
            }
        }
    }

    for (size_t i = 0; i < pendSize; ++i) {
        if (std::find(insertIndices.begin(), insertIndices.end(), i) ==
            insertIndices.end()) {
            insertIndices.push_back(i);
        }
    }

    return insertIndices;
}

template <typename Container>
void PmergeMe<Container>::sort() {
    mergePairs();

    if (pairs.empty()) {
        return;
    }
    Container pend;

    for (auto& pair : pairs) {
        auto& [smaller, larger] = pair;

        for (int large : larger) {
            auto pos = mainChain.begin();
            while (pos != mainChain.end() && *pos < large) {
                ++pos;
            }
            mainChain.insert(pos, large);
        }
        for (int small : smaller) {
            pend.push_back(small);
        }
    }

    for (int straggler : stragglers) {
        pend.push_back(straggler);
    }

    Container jacobsthal = generateJacobsthalSequence(pend.size());
    Container insertIndices = calculateInsertionOrder(jacobsthal, pend.size());

    for (int idx : insertIndices) {
        auto it = pend.begin();
        std::advance(it, idx);
        int elementToInsert = *it;

        binaryInsert(elementToInsert);
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
void PmergeMe<Container>::printList(const Container& list) const {
    std::cout << '[';
    bool first = true;
    for (auto i : list) {
        if (!first) {
            std::cout << ',';
        }
        std::cout << i;
        first = false;
    }
    std::cout << ']';
}

template <typename Container>
void PmergeMe<Container>::printPairs() const {
    bool first = true;
    for (auto& pair : pairs) {
        const auto& [left, right] = pair;
        if (!first) {
            std::cout << ' ';
        }

        std::cout << '[';
        if (left.size() == 1) {
            std::cout << left.front();
        } else {
            printList(left);
        }

        std::cout << ',';

        if (right.size() == 1) {
            std::cout << right.front();
        } else {
            printList(right);
        }
        std::cout << ']';
        first = false;
    }
}
