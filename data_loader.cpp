#include "data_loader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>

EcofunctionalExperiment DataLoader::loadExperimentFromCSV(const std::string& filepath, const std::string& experimentId) {
    EcofunctionalExperiment experiment(experimentId);
    
    // Reusing the simple loadCSV from dataset.cpp would require adaptation since columns might differ.
    // Implementing a specific parser here for the Domain CSV format.
    
    std::ifstream file(filepath);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open training CSV: " + filepath);
    }
    std::string line;
    
    // Skip Header
    if (file.good()) std::getline(file, line);

    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string val;
        std::vector<float> row;
        
        // CSV Format: SoildDepth, SoilComp, ..., Propagule, Target
        while (std::getline(ss, val, ',')) {
            try {
                row.push_back(std::stof(val));
            } catch (const std::exception&) {
                std::cerr << "[DataLoader] Skipping invalid value '" << val << "'\n";
            }
        }

        if (row.size() >= 11) { // 10 features + 1 target
            EcofunctionalVector vec = EcofunctionalVector::fromVector({row.begin(), row.begin() + 10});
            float target = row[10];
            experiment.addSample({vec, target});
        } else {
            std::cerr << "[DataLoader] Skipping line with insufficient columns: " << row.size() << std::endl;
        }
    }

    std::cout << "[DataLoader] Loaded " << experiment.getSamples().size() << " samples for experiment " << experimentId << std::endl;
    return experiment;
}

EcofunctionalTrajectory DataLoader::loadTrajectoryFromCSV(const std::string& filepath) {
    EcofunctionalTrajectory traj;
    std::ifstream file(filepath);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open trajectory CSV: " + filepath);
    }
    std::string line;
    
    // Skip Header
    if (file.good()) std::getline(file, line);

    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string val;
        std::vector<float> row;
        
        while (std::getline(ss, val, ',')) {
             try {
                row.push_back(std::stof(val));
            } catch (const std::exception&) { 
                std::cerr << "[DataLoader] Skipping invalid value '" << val << "'\n";
            }
        }

        if (row.size() >= 10) { 
            // For inference, we might not have target, use 0.0 default
            EcofunctionalVector vec = EcofunctionalVector::fromVector({row.begin(), row.begin() + 10});
            traj.addSample({vec, 0.0f});
        } else {
            std::cerr << "[DataLoader] Skipping line with insufficient columns: " << row.size() << std::endl;
        }
    }
    std::cout << "[DataLoader] Loaded trajectory with " << traj.history.size() << " steps." << std::endl;
    return traj;
}
