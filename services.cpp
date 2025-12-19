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
    
    InferenceOutput output;
    output.functionalIntegrity = rawOutput;
    
    // =========================================================
    // PHASE 3: Continuous Recovery & Hysteresis
    // =========================================================
    
    // Analyze trajectory state
    auto state = trajectory.analyzeState();
    float vegTrend = trajectory.getVegetationTrend();
    
    // Logic for Recovery Capacity (0.0 - 1.0)
    // 1. High Integrity + Stable = Climax (High Resilience)
    // 2. High Integrity + Positive Trend = Robust Recovery (Very High Resilience)
    // 3. Low Integrity + Positive Trend = Early Recovery (Medium Resilience)
    // 4. Negative Trend = Collapsing (Low Resilience)
    
    if (rawOutput > 0.7f) {
        if (state == EcofunctionalTrajectory::TrajectoryState::RECOVERING) {
            output.recoveryCapacity = 1.0f; // Robust, active recovery
        } else if (state == EcofunctionalTrajectory::TrajectoryState::STABLE) {
            output.recoveryCapacity = 0.9f; // Mature/Climax
        } else {
            output.recoveryCapacity = 0.7f; // Declining high state
        }
    } else {
        if (state == EcofunctionalTrajectory::TrajectoryState::RECOVERING) {
            output.recoveryCapacity = 0.5f + (vegTrend * 2.0f); // Variable early recovery
            if (output.recoveryCapacity > 0.8f) output.recoveryCapacity = 0.8f;
        } else {
             output.recoveryCapacity = 0.1f; // Degraded and static/worsening
        }
    }
    
    // Resilience Potential correlates with both Integrity and Capacity
    output.resiliencePotential = (output.functionalIntegrity + output.recoveryCapacity) / 2.0f;
    
    return output;
}
