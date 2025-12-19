#pragma once
#include <string>
#include <vector>
#include "domain.h"
#include "dataset.h" // Reusing low-level CSV util

class DataLoader {
public:
    // Loads a CSV where each row is a time-step.
    // Expected Format: ID, SoilDepth, HydroFlux, ... (10 features), Target(Optional)
    static EcofunctionalExperiment loadExperimentFromCSV(const std::string& filepath, const std::string& experimentId);

    // Loads a specific single trajectory (e.g. for inference)
    static EcofunctionalTrajectory loadTrajectoryFromCSV(const std::string& filepath);
};
