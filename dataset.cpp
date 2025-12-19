#include "dataset.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>

Dataset loadCSV(const std::string& path) {
    Dataset ds;
    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open CSV: " + path);
    }

    std::string line;
    bool skippedHeader = false;

    while (std::getline(file, line)) {
        if (!skippedHeader) { // assume first line is header
            skippedHeader = true;
            continue;
        }
        if (line.empty()) continue;

        std::stringstream ss(line);
        std::vector<float> row;
        std::string value;

        while (std::getline(ss, value, ',')) {
            try {
                row.push_back(std::stof(value));
            } catch (const std::exception& e) {
                std::cerr << "[loadCSV] Skipping invalid value '" << value << "'\n";
            }
        }

        if (row.size() < 2) {
            std::cerr << "[loadCSV] Skipping line with insufficient columns\n";
            continue;
        }

        ds.y.push_back(row.back());
        row.pop_back();
        ds.X.push_back(row);
    }
    return ds;
}
