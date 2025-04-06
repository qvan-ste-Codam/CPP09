#include "BitcoinExchange.hpp"

#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

std::time_t strToTime(std::string& date) {
    std::tm tm = {};
    std::stringstream ymd(date);

    ymd >> std::get_time(&tm, "%Y-%m-%d");
    if (ymd.fail()) {
        throw std::runtime_error("not a valid date => " + date);
    }

    std::string extraChars;
    if (ymd >> extraChars) {
        throw std::runtime_error("not a valid date => " + date);
    }
    std::time_t time = std::mktime(&tm);
    return time;
}

float safeToFloat(const std::string& input) {
    try {
        size_t pos = 0;
        float f = std::stof(input, &pos);

        if (pos != input.length()) {
            throw std::runtime_error("not a valid float => " + input);
        }
        return f;
    } catch (const std::exception& e) {
        throw std::runtime_error("not a valid float => " + input);
    }
}

BitcoinExchange::BitcoinExchange(const std::string& filePath) : data() {
    std::ifstream dataFile(filePath);
    if (!dataFile.is_open()) {
        throw std::runtime_error("failed to open data file => " + filePath);
    }

    try {
        std::string line;
        std::getline(dataFile, line);
        if (line != "date,exchange_rate") {
            throw std::runtime_error("not a valid data header => " + line);
        }

        while (std::getline(dataFile, line)) {
            std::vector<std::string> row;
            std::stringstream ss(line);
            std::string cell;

            while (std::getline(ss, cell, ',')) {
                row.push_back(cell);
            }
            if (row.size() != 2) {
                throw std::runtime_error("not a valid format => " + line);
            }
            std::string date = row[0];
            std::string value = row[1];

            auto timePoint = strToTime(date);
            auto floatValue = safeToFloat(value);
            data[timePoint] = floatValue;
        }
        dataFile.close();
    } catch (...) {
        dataFile.close();
        throw;
    }
}

BitcoinExchange::BitcoinExchange(const BitcoinExchange& other)
    : data(other.data) {}

BitcoinExchange& ::BitcoinExchange::operator=(const BitcoinExchange & other) {
    if (this != &other) {
        data = other.data;
    }
    return *this;
}

void BitcoinExchange::processInputRow(
    const std::vector<std::string>& row) const {
    std::time_t tp;
    float value;
    std::string date = row[0];
    std::string delim = row[1];
    std::string valueString = row[2];

    if (delim != "|") {
        std::cerr << "Error: not a valid delimiter => " << delim << std::endl;
        return;
    }
    try {
        tp = strToTime(date);
        value = safeToFloat(valueString);
    } catch (const std::runtime_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return;
    }
    if (value < 0) {
        std::cerr << "Error: not a postive number => " << valueString
                  << std::endl;
        return;
    }
    if (value > 1000) {
        std::cerr << "Error: number too large => " << valueString << std::endl;
        return;
    }
    auto it = data.lower_bound(tp);
    if (it == data.end()) {
        std::cerr << "Error: date is after last data entry => " << date
                  << std::endl;
        return;
    }
    if (it->first > tp && it != data.begin()) {
        it--;
    }
    float rate = it->second;
    std::cout << std::setprecision(2);
    std::cout << date << " => " << value * rate << std::endl;
}

void BitcoinExchange::calculateRates(const std::string& filePath) const {
    std::filesystem::path fs(filePath);
    if (fs.extension() != ".txt") {
        throw std::runtime_error("not a valid file => " + filePath);
    }

    std::ifstream inputFile(filePath);
    if (!inputFile.is_open()) {
        throw std::runtime_error("cannot open file => " + filePath);
    }

    try {
        std::string line;
        std::getline(inputFile, line);
        if (line != "date | value") {
            throw std::runtime_error("not a valid input header => " + line);
        }
        while (std::getline(inputFile, line)) {
            std::vector<std::string> row;
            std::stringstream ss(line);
            std::string cell;

            while (std::getline(ss, cell, ' ')) {
                row.push_back(cell);
            }
            if (row.size() != 3) {
                std::cerr << "Error: not a valid format => " << line
                          << std::endl;
                continue;
            }
            processInputRow(row);
        }
        inputFile.close();
    } catch (...) {
        inputFile.close();
        throw;
    }
}
