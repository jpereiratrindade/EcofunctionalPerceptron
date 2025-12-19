#include "domain.h"
#include <stdexcept>
#include <numeric>

// ==========================================
// EcofunctionalVector
// ==========================================

std::vector<float> EcofunctionalVector::toVector() const {
    return {
        soilDepth,
        soilCompaction,
        soilInfiltration,
        hydroFlux,
        erosionRisk,
        vegetationCoverageEI,
        vegetationCoverageES,
        vegetationVigorEI,
        vegetationVigorES,
        propagulePotential
    };
}

EcofunctionalVector EcofunctionalVector::fromVector(const std::vector<float>& vec) {
    if (vec.size() != 10) {
        throw std::invalid_argument("Vector size must be 10 for EcofunctionalVector");
    }
    return EcofunctionalVector{
        vec[0], vec[1], vec[2], vec[3], vec[4],
        vec[5], vec[6], vec[7], vec[8], vec[9]
    };
}

EcofunctionalVector EcofunctionalVector::operator-(const EcofunctionalVector& other) const {
    return {
        soilDepth - other.soilDepth,
        soilCompaction - other.soilCompaction,
        soilInfiltration - other.soilInfiltration,
        hydroFlux - other.hydroFlux,
        erosionRisk - other.erosionRisk,
        vegetationCoverageEI - other.vegetationCoverageEI,
        vegetationCoverageES - other.vegetationCoverageES,
        vegetationVigorEI - other.vegetationVigorEI,
        vegetationVigorES - other.vegetationVigorES,
        propagulePotential - other.propagulePotential
    };
}

EcofunctionalVector EcofunctionalVector::operator+(const EcofunctionalVector& other) const {
    return {
        soilDepth + other.soilDepth,
        soilCompaction + other.soilCompaction,
        soilInfiltration + other.soilInfiltration,
        hydroFlux + other.hydroFlux,
        erosionRisk + other.erosionRisk,
        vegetationCoverageEI + other.vegetationCoverageEI,
        vegetationCoverageES + other.vegetationCoverageES,
        vegetationVigorEI + other.vegetationVigorEI,
        vegetationVigorES + other.vegetationVigorES,
        propagulePotential + other.propagulePotential
    };
}

EcofunctionalVector EcofunctionalVector::operator/(float scalar) const {
    return {
        soilDepth / scalar,
        soilCompaction / scalar,
        soilInfiltration / scalar,
        hydroFlux / scalar,
        erosionRisk / scalar,
        vegetationCoverageEI / scalar,
        vegetationCoverageES / scalar,
        vegetationVigorEI / scalar,
        vegetationVigorES / scalar,
        propagulePotential / scalar
    };
}

// ==========================================
// EcofunctionalTrajectory
// ==========================================

void EcofunctionalTrajectory::addSample(const EcofunctionalSample& sample) {
    history.push_back(sample);
}

EcofunctionalVector EcofunctionalTrajectory::calculateDelta() const {
    if (history.size() < 2) {
        return EcofunctionalVector{0}; // No change if not enough history
    }
    const auto& last = history.back().inputVector;
    const auto& prev = history[history.size() - 2].inputVector;
    return last - prev;
}

EcofunctionalVector EcofunctionalTrajectory::calculateAverage(int windowSize) const {
    if (history.empty()) return EcofunctionalVector{0};
    
    int count = 0;
    EcofunctionalVector sum{0};
    
    // Iterate backwards
    for (int i = history.size() - 1; i >= 0 && count < windowSize; --i) {
        sum = sum + history[i].inputVector;
        count++;
    }
    
    return sum / static_cast<float>(count);
}

float EcofunctionalTrajectory::getVegetationTrend() const {
    EcofunctionalVector delta = calculateDelta();
    // Simplified aggregated trend for vegetation
    return (delta.vegetationCoverageEI + delta.vegetationCoverageES + 
            delta.vegetationVigorEI + delta.vegetationVigorES) / 4.0f;
}

float EcofunctionalTrajectory::getHydroTrend() const {
    EcofunctionalVector delta = calculateDelta();
    // Hydro trend: +HydroFlux is usually good, +Infiltration is good.
    return (delta.hydroFlux + delta.soilInfiltration) / 2.0f;
}

EcofunctionalTrajectory::TrajectoryState EcofunctionalTrajectory::analyzeState() const {
    if (history.size() < 2) return TrajectoryState::UNKNOWN;

    float vegTrend = getVegetationTrend();
    float totalDeltaMagnitude = 0; // sum of absolute deltas could be used to detect stability
    
    // Heuristic Thresholds (To be calibrated)
    const float STABILITY_THRESHOLD = 0.01f;
    const float RECOVERY_THRESHOLD = 0.05f;

    if (std::abs(vegTrend) < STABILITY_THRESHOLD) {
        return TrajectoryState::STABLE;
    } else if (vegTrend > RECOVERY_THRESHOLD) {
        return TrajectoryState::RECOVERING;
    } else if (vegTrend < -RECOVERY_THRESHOLD) {
        return TrajectoryState::COLLAPSING;
    } else if (vegTrend < 0) {
        return TrajectoryState::DEGRADING;
    }
    
    return TrajectoryState::STABLE; // Default fallback
}


// ==========================================
// EcofunctionalExperiment
// ==========================================

EcofunctionalExperiment::EcofunctionalExperiment(const std::string& id)
    : experimentId_(id) {}

void EcofunctionalExperiment::addSample(const EcofunctionalSample& sample) {
    samples_.push_back(sample);
}

const std::vector<EcofunctionalSample>& EcofunctionalExperiment::getSamples() const {
    return samples_;
}

std::string EcofunctionalExperiment::getId() const {
    return experimentId_;
}
