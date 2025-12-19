#include <iostream>
#include <vector>
#include <fstream> 
#include "domain.h"
#include "services.h"
#include "perceptron.h"
#include "data_loader.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

void printResult(const std::string& label, const InferenceOutput& res) {
    std::cout << "\n[" << label << "]" << std::endl;
    std::cout << "  Functional Integrity: " << res.functionalIntegrity << std::endl;
    std::cout << "  Recovery Capacity:    " << res.recoveryCapacity << " (Continuous 0-1)" << std::endl;
    std::cout << "  Resilience Potential: " << res.resiliencePotential << std::endl;
}

// Save results to JSON for python plotting
void saveInferenceLog(const std::string& filename, const std::vector<InferenceOutput>& history) {
    json j;
    std::vector<json> entries;
    int t = 0;
    for (const auto& log : history) {
        entries.push_back({
            {"step", t++},
            {"functional_integrity", log.functionalIntegrity},
            {"recovery_capacity", log.recoveryCapacity},
            {"resilience_potential", log.resiliencePotential}
        });
    }
    j["history"] = entries;
    std::ofstream o(filename);
    o << j.dump(4);
}

int main() {
    std::cout << "=== Ecofunctional Perceptron Experiment v0.2.0 (CSV Pipeline) ===\n" << std::endl;

    // 1. Load Training Data
    auto experiment = DataLoader::loadExperimentFromCSV("training_data.csv", "EXP-CSV-01");

    // 2. Train Model
    Perceptron model(ECOFEATURE_VECTOR_SIZE);
    PerceptronTrainingService trainer;
    trainer.trainFullExperiment(model, experiment, 0.1f, 2000);

    // 3. Load Trajectory for Inference
    auto trajectory = DataLoader::loadTrajectoryFromCSV("trajectory_data.csv");

    // 4. Run Sequential Inference (Piecewise Analysis)
    // To visualize hysteresis, we evaluate the trajectory step-by-step
    PerceptronInferenceService inferenceService;
    std::vector<InferenceOutput> resultsLog;

    std::cout << "\n--> Running Sequential Inference on Trajectory..." << std::endl;
    
    // Create growing sub-trajectories to simulate time passing
    EcofunctionalTrajectory simulationTraj;
    for (const auto& sample : trajectory.history) {
        simulationTraj.addSample(sample);
        
        auto res = inferenceService.inferState(model, simulationTraj);
        resultsLog.push_back(res);
        
        std::cout << "Step " << simulationTraj.history.size() 
                  << ": Integrity=" << res.functionalIntegrity 
                  << ", Recovery=" << res.recoveryCapacity << std::endl;
    }

    // 5. Save Results for Visualization
    saveInferenceLog("inference_results.json", resultsLog);
    std::cout << "\nResults saved to 'inference_results.json'. Run plotting script!" << std::endl;

    return 0;
}
