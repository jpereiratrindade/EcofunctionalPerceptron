#include "dataset.h"
#include <fstream>
#include <sstream>

Dataset loadCSV(const std::string& path) {
    Dataset ds;
    std::ifstream file(path);
    std::string line;

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::vector<float> row;
        std::string value;

        while (std::getline(ss, value, ','))
            row.push_back(std::stof(value));

        ds.y.push_back(row.back());
        row.pop_back();
        ds.X.push_back(row);
    }
    return ds;
}
