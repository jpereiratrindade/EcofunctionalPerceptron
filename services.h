#pragma once
#include "domain.h"
#include "perceptron.h"
#include "dataset.h"

constexpr size_t ECOFEATURE_VECTOR_SIZE = 30; // current state (10) + delta (10) + rolling average (10)

// ==========================================
// Domain Services
// ==========================================

class PerceptronTrainingService {
public:
    // Trains a perceptron model using the experiment's data
    void trainFullExperiment(Perceptron& model, 
                             const EcofunctionalExperiment& experiment, 
                             float learningRate, 
                             int epochs);
};

class PerceptronInferenceService {
public:
    // Uses a trained model to infer ecofunctional properties
    InferenceOutput inferState(const Perceptron& model, 
                               const EcofunctionalTrajectory& trajectory);
};
