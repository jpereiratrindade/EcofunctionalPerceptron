#include <iostream>
#include <vector>
#include "domain.h"
#include "services.h"
#include "perceptron.h"

// Helper to print Ecofunctional Vector
void printVector(const EcofunctionalVector& vec) {
    std::cout << "  - Soil Depth: " << vec.soilDepth << "\n"
              << "  - Hydro Flux: " << vec.hydroFlux << "\n"
              << "  - Veg Coverage EI: " << vec.vegetationCoverageEI << "\n"
              << "  - Propagule Pot: " << vec.propagulePotential << "\n";
}

int main() {
    std::cout << "=== Ecofunctional Perceptron Experiment ===\n" << std::endl;

    // 1. Setup Experiment Aggregate
    EcofunctionalExperiment experiment("EXP-2025-ALPHA");

    // 2. Add Synthetic Data (simulating Ecological Subdomain snapshots)
    std::cout << "--> Loading Samples..." << std::endl;
    
    // Sample 1: Healthy Ecosystem (High integrity target)
    experiment.addSample({
        EcofunctionalVector{1.0, 0.2, 0.8, 0.5, 0.1, 0.9, 0.9, 0.8, 0.8, 0.9},
        1.0f // Target: High Functional Integrity
    });

    // Sample 2: Degraded Ecosystem (Low integrity target)
    experiment.addSample({
        EcofunctionalVector{0.2, 0.9, 0.1, 0.9, 0.8, 0.1, 0.1, 0.2, 0.2, 0.1},
        0.0f // Target: Low Functional Integrity
    });

    // Sample 3: Recovering Ecosystem (Medium)
    experiment.addSample({
        EcofunctionalVector{0.5, 0.5, 0.5, 0.5, 0.4, 0.5, 0.4, 0.6, 0.5, 0.6},
        0.6f 
    });

    // 3. Initialize Domain Service and Infrastructure
    Perceptron model(10); // 10 inputs matching EcofunctionalVector
    PerceptronTrainingService trainer;
    
    // 4. Run Training
    std::cout << "--> Training Model..." << std::endl;
    trainer.trainFullExperiment(model, experiment, 0.1f, 1000);

    // 5. Run Inference on a Trajectory (Time Series)
    PerceptronInferenceService inferenceService;
    
    std::cout << "\n--> Running Inference on Trajectory..." << std::endl;
    
    EcofunctionalTrajectory trajectory;
    
    // t0: Initial State
    trajectory.addSample({EcofunctionalVector{0.5, 0.3, 0.7, 0.4, 0.2, 0.8, 0.8, 0.7, 0.9, 0.85}, 0});
    // t1: Improvement (Positive Delta)
    trajectory.addSample({EcofunctionalVector{0.6, 0.3, 0.7, 0.4, 0.2, 0.8, 0.8, 0.7, 0.9, 0.85}, 0}); 
    // t2: More Improvement (Current State)
    EcofunctionalVector currentState = {0.8, 0.3, 0.7, 0.4, 0.2, 0.8, 0.8, 0.7, 0.9, 0.85};
    trajectory.addSample({currentState, 0});

    std::cout << "Current State (t2):" << std::endl;
    printVector(currentState);

    EcofunctionalVector delta = trajectory.calculateDelta();
    std::cout << "Delta (t2 - t1): Soil Depth change = " << delta.soilDepth << std::endl;

    InferenceOutput result = inferenceService.inferState(model, trajectory);

    std::cout << "\n[RESULT]" << std::endl;
    std::cout << "Functional Integrity: " << result.functionalIntegrity << " (Raw Probability)" << std::endl;
    std::cout << "Resilience Potential: " << result.resiliencePotential << std::endl;
    std::cout << "Recovery Capacity:    " << result.recoveryCapacity << std::endl;

    // 6. Save Model Artifact
    model.save("ecofunc_model.json");
    std::cout << "\nModel artifact saved to 'ecofunc_model.json'." << std::endl;

    return 0;
}
