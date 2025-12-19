#pragma once
#include <vector>
#include <string>

class Perceptron {
public:
    Perceptron(size_t inputSize);

    float infer(const std::vector<float>& x) const;
    void train(const std::vector<std::vector<float>>& X,
               const std::vector<float>& y,
               float lr,
               int epochs);

    void save(const std::string& path) const;
    void load(const std::string& path);

private:
    std::vector<float> weights_;
    float bias_;

    static float sigmoid(float z);
};
