#include "services.h"
#include <iostream>

// ==========================================
// PerceptronTrainingService
// ==========================================

void PerceptronTrainingService::trainFullExperiment(Perceptron& model, 
                                                    const EcofunctionalExperiment& experiment, 
                                                    float learningRate, 
                                                    int epochs) {
    std::cout << "[Service] Starting training for Experiment: " << experiment.getId() << std::endl;
    
    std::vector<std::vector<float>> X;
    std::vector<float> y;
    
    const auto& samples = experiment.getSamples();
    for (const auto& sample : samples) {
        X.push_back(sample.inputVector.toVector());
        y.push_back(sample.targetLabel);
    }
    
    model.train(X, y, learningRate, epochs);
    std::cout << "[Service] Training completed." << std::endl;
}

// ==========================================
// PerceptronInferenceService
// ==========================================


InferenceOutput PerceptronInferenceService::inferState(const Perceptron& model, 
                                                       const EcofunctionalTrajectory& trajectory) {
    if (trajectory.history.empty()) return {};

    // FEATURE ENGINEERING STRATEGY
    // Input Vector = [Current State (10)] + [Delta (10)] + [Avg3 (10)]
    // Total Input Size expected by Perceptron = 30
    
    EcofunctionalVector current = trajectory.history.back().inputVector;
    EcofunctionalVector delta = trajectory.calculateDelta();
    EcofunctionalVector avg = trajectory.calculateAverage(3);

    std::vector<float> inputFeatures;
    auto vCurr = current.toVector();
    auto vDelta = delta.toVector();
    auto vAvg = avg.toVector();

    inputFeatures.insert(inputFeatures.end(), vCurr.begin(), vCurr.end());
    // For now, to keep using the existing 10-input perceptron without breaking everything immediately,
    // we will simple WEIGHT the current input based on the trend.
    // Ideally we would expand the Perceptron input size to 30.
    
    // TEMPORARY LOGIC: Modulating the raw input based on positive/negative trends
    // If the trend (delta) is positive, we slightly boost the input signal.
    
    std::vector<float> modulatedInput = vCurr;
    for(size_t i=0; i<10; ++i) {
        modulatedInput[i] += vDelta[i] * 0.5f; // Add half of the delta momentum
    }

    float rawOutput = model.infer(modulatedInput);
    
    // Mapping raw perceptron output (0-1) to Domain Semantics
    // Note: In a real complex model, these might be separate outputs or more complex logic.
    // Here we assume the single output approximates 'Functional Integrity'.
    
    InferenceOutput output;
    output.functionalIntegrity = rawOutput;
    
    // Deriving other metrics based on integrity (hypothetical logic for this subdomain)
    output.resiliencePotential = rawOutput * 0.9f; // Simplified correlation
    output.recoveryCapacity = (rawOutput > 0.5f) ? 1.0f : 0.2f; 
    
    return output;
}
