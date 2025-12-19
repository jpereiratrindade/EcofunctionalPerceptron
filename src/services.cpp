#include "services.h"
#include <iostream>
#include <stdexcept>

namespace {
std::vector<float> buildFeatureVector(const EcofunctionalTrajectory& trajectory) {
    if (trajectory.history.empty()) return {};

    EcofunctionalVector current = trajectory.history.back().inputVector;
    EcofunctionalVector delta = trajectory.calculateDelta();
    EcofunctionalVector avg = trajectory.calculateAverage(3);

    std::vector<float> features;
    auto vCurr = current.toVector();
    auto vDelta = delta.toVector();
    auto vAvg = avg.toVector();

    features.reserve(vCurr.size() * 3);
    features.insert(features.end(), vCurr.begin(), vCurr.end());
    features.insert(features.end(), vDelta.begin(), vDelta.end());
    features.insert(features.end(), vAvg.begin(), vAvg.end());

    return features;
}
} // namespace

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
    EcofunctionalTrajectory trajectory;
    
    const auto& samples = experiment.getSamples();
    if (samples.empty()) {
        throw std::runtime_error("No samples found in experiment for training");
    }

    for (const auto& sample : samples) {
        trajectory.addSample(sample);
        auto features = buildFeatureVector(trajectory);
        if (features.size() != ECOFEATURE_VECTOR_SIZE) {
            throw std::runtime_error("Unexpected feature size during training");
        }
        X.push_back(std::move(features));
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
    auto inputFeatures = buildFeatureVector(trajectory);
    float rawOutput = model.infer(inputFeatures);
    
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
