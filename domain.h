#pragma once
#include <vector>
#include <string>
#include <iostream>

// ==========================================
// Value Objects
// ==========================================

struct EcofunctionalVector {
    float soilDepth;
    float soilCompaction;
    float soilInfiltration;
    float hydroFlux;
    float erosionRisk;
    float vegetationCoverageEI;
    float vegetationCoverageES;
    float vegetationVigorEI;
    float vegetationVigorES;
    float propagulePotential;

    std::vector<float> toVector() const;
    static EcofunctionalVector fromVector(const std::vector<float>& vec);

    // Operator overloads for convenient math
    EcofunctionalVector operator-(const EcofunctionalVector& other) const;
    EcofunctionalVector operator+(const EcofunctionalVector& other) const;
    EcofunctionalVector operator/(float scalar) const;
};

// ==========================================
// Entities
// ==========================================

struct EcofunctionalSample {
    EcofunctionalVector inputVector;
    float targetLabel; // Simplified target for single-output perceptron (e.g. Integrity)
};

struct EcofunctionalTrajectory {
    std::vector<EcofunctionalSample> history;

    void addSample(const EcofunctionalSample& sample);
    
    // Feature Engineering Methods
    EcofunctionalVector calculateAverage(int windowSize) const;
    EcofunctionalVector calculateDelta() const; // Last - Penultimate

    // Ecological Analysis
    enum class TrajectoryState {
        STABLE,
        RECOVERING,
        DEGRADING,
        COLLAPSING,
        UNKNOWN
    };

    TrajectoryState analyzeState() const;
    float getVegetationTrend() const;
    float getHydroTrend() const;
};

struct InferenceOutput {
    float resiliencePotential;
    float functionalIntegrity;
    float recoveryCapacity;
};

// ==========================================
// Aggregate Root
// ==========================================

class EcofunctionalExperiment {
public:
    EcofunctionalExperiment(const std::string& id);

    void addSample(const EcofunctionalSample& sample);
    const std::vector<EcofunctionalSample>& getSamples() const;
    std::string getId() const;

private:
    std::string experimentId_;
    std::vector<EcofunctionalSample> samples_;
};
