#pragma once
#include <ctime>
#include <map>
#include <string>

class BitcoinExchange {
   public:
    BitcoinExchange(const std::string &filePath = "data.csv");
    BitcoinExchange(const BitcoinExchange &other);
    ~BitcoinExchange() = default;
    BitcoinExchange &operator=(const BitcoinExchange &other);

    void calculateRates(const std::string &filePath) const;

   private:
    std::map<std::time_t, float> data;

    void processInputRow(const std::string &date,
                         const std::string &valueString) const;
};
