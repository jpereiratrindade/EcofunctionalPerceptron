#include "perceptron.h"
#include <cmath>
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

Perceptron::Perceptron(size_t inputSize)
    : weights_(inputSize, 0.0f), bias_(0.0f) {}

float Perceptron::sigmoid(float z) {
    return 1.0f / (1.0f + std::exp(-z));
}

float Perceptron::infer(const std::vector<float>& x) const {
    float z = bias_;
    for (size_t i = 0; i < x.size(); ++i)
        z += weights_[i] * x[i];
    return sigmoid(z);
}

void Perceptron::train(const std::vector<std::vector<float>>& X,
                       const std::vector<float>& y,
                       float lr,
                       int epochs) {
    for (int e = 0; e < epochs; ++e) {
        for (size_t i = 0; i < X.size(); ++i) {
            float y_hat = infer(X[i]);
            float error = y[i] - y_hat;

            for (size_t j = 0; j < weights_.size(); ++j)
                weights_[j] += lr * error * X[i][j];

            bias_ += lr * error;
        }
    }
}

void Perceptron::save(const std::string& path) const {
    json j;
    j["weights"] = weights_;
    j["bias"] = bias_;

    std::ofstream file(path);
    file << j.dump(4);
}

void Perceptron::load(const std::string& path) {
    std::ifstream file(path);
    json j;
    file >> j;

    weights_ = j["weights"].get<std::vector<float>>();
    bias_ = j["bias"].get<float>();
}
