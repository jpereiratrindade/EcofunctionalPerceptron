#pragma once
#include <vector>
#include <string>

struct Dataset {
    std::vector<std::vector<float>> X;
    std::vector<float> y;
};

Dataset loadCSV(const std::string& path);
