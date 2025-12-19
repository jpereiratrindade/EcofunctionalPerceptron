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

void printResult(const std::string& label, const InferenceOutput& res) {
    std::cout << "\n[" << label << "]" << std::endl;
    std::cout << "  Functional Integrity: " << res.functionalIntegrity << std::endl;
    std::cout << "  Recovery Capacity:    " << res.recoveryCapacity << " (Continuous 0-1)" << std::endl;
    std::cout << "  Resilience Potential: " << res.resiliencePotential << std::endl;
}

int main() {
    std::cout << "=== Ecofunctional Perceptron Experiment v0.1.0 ===\n" << std::endl;

    // 1. Setup Experiment & Training Data
    EcofunctionalExperiment experiment("EXP-PHASE-3");
    experiment.addSample({EcofunctionalVector{1.0, 0.2, 0.8, 0.5, 0.1, 0.9, 0.9, 0.8, 0.8, 0.9}, 1.0f}); // Healthy
    experiment.addSample({EcofunctionalVector{0.2, 0.9, 0.1, 0.9, 0.8, 0.1, 0.1, 0.2, 0.2, 0.1}, 0.0f}); // Degraded

    Perceptron model(10);
    PerceptronTrainingService trainer;
    trainer.trainFullExperiment(model, experiment, 0.1f, 1000); // Quick training

    PerceptronInferenceService inferenceService;

    // 2. Scenario A: STABLE HIGH (Climax)
    // High state, barely changing
    EcofunctionalTrajectory trajStable;
    EcofunctionalVector highState = {0.9, 0.2, 0.8, 0.5, 0.1, 0.9, 0.9, 0.8, 0.8, 0.9};
    trajStable.addSample({highState, 0});
    trajStable.addSample({highState, 0}); 
    trajStable.addSample({highState, 0}); // No change
    
    auto resStable = inferenceService.inferState(model, trajStable);
    printResult("SCENARIO A: STABLE HIGH (Climax)", resStable);

    // 3. Scenario B: ACTIVE RECOVERY
    // Similar high state, but showing specific positive trend in vegetation
    EcofunctionalTrajectory trajRec;
    trajRec.addSample({EcofunctionalVector{0.7, 0.2, 0.8, 0.5, 0.1, 0.7, 0.7, 0.6, 0.6, 0.9}, 0});
    trajRec.addSample({EcofunctionalVector{0.8, 0.2, 0.8, 0.5, 0.1, 0.8, 0.8, 0.7, 0.7, 0.9}, 0});
    trajRec.addSample({EcofunctionalVector{0.9, 0.2, 0.8, 0.5, 0.1, 0.9, 0.9, 0.8, 0.8, 0.9}, 0}); // Reached high state

    auto resRec = inferenceService.inferState(model, trajRec);
    printResult("SCENARIO B: ACTIVE RECOVERY", resRec);

     // 4. Scenario C: DEGRADED STATIC
    EcofunctionalTrajectory trajDeg;
    EcofunctionalVector lowState = {0.2, 0.9, 0.1, 0.9, 0.8, 0.1, 0.1, 0.2, 0.2, 0.1};
    trajDeg.addSample({lowState, 0});
    trajDeg.addSample({lowState, 0});
    trajDeg.addSample({lowState, 0}); 

    auto resDeg = inferenceService.inferState(model, trajDeg);
    printResult("SCENARIO C: DEGRADED STATIC", resDeg);

    return 0;
}
