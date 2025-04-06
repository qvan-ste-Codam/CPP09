#pragma once
#include <ctime>
#include <string>
#include <map>
#include <vector>

class BitcoinExchange {
   public:
    BitcoinExchange(const std::string &filePath = "data.csv");
    BitcoinExchange(const BitcoinExchange &other);
    ~BitcoinExchange() = default;
    BitcoinExchange &operator=(const BitcoinExchange &other);

    void calculateRates(const std::string &filePath) const;

   private:
    std::map<std::time_t, float> data;

    void processInputRow(const std::vector<std::string> &row) const;
};
