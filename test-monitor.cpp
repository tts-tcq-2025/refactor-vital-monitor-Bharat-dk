#include <iostream>
#include <string>
#include <vector>
#include <optional>
#include <unordered_map>
#include <functional>

// -----------------------------
// Data Structures
// -----------------------------

struct VitalLimits {
    float low;
    float high;
    bool inclusive; // true: value < low || value > high
};

struct VitalReading {
    std::string name;
    float value;
};

// -----------------------------
// Pure Functions
// -----------------------------

bool isOutOfRange(float value, const VitalLimits& limits) {
    if (limits.inclusive) {
        return value < limits.low || value > limits.high;
    }
    return value <= limits.low || value >= limits.high;
}

std::optional<std::string> createAlertMessage(const VitalReading& reading) {
    return reading.name + " is out of range!";
}

// -----------------------------
// Limits Repository (Pure)
// -----------------------------

class VitalLimitsRepository {
public:
    // You can inject different rules here for different patients (e.g., age, condition)
    VitalLimitsRepository() {
        vitalLimitsMap = {
            {"Temperature", VitalLimits{95.0f, 102.0f, true}},
            {"PulseRate",   VitalLimits{60.0f, 100.0f, true}},
            {"SpO2",        VitalLimits{90.0f, 200.0f, true}}
        };
    }

    std::optional<VitalLimits> getLimits(const std::string& vitalName) const {
        auto it = vitalLimitsMap.find(vitalName);
        if (it != vitalLimitsMap.end()) {
            return it->second;
        }
        return std::nullopt;
    }

private:
    std::unordered_map<std::string, VitalLimits> vitalLimitsMap;
};

// -----------------------------
// Business Logic (Pure)
// -----------------------------

class VitalChecker {
public:
    explicit VitalChecker(const VitalLimitsRepository& repo) : limitsRepo(repo) {}

    std::optional<std::string> check(const VitalReading& reading) const {
        auto limitsOpt = limitsRepo.getLimits(reading.name);
        if (!limitsOpt.has_value()) {
            return "Unknown vital: " + reading.name;
        }

        if (isOutOfRange(reading.value, limitsOpt.value())) {
            return createAlertMessage(reading);
        }

        return std::nullopt;
    }

private:
    const VitalLimitsRepository& limitsRepo;
};

// -----------------------------
// I/O Logic (Separate)
// -----------------------------

void showAlert(const std::string& message) {
    std::cout << message << std::endl;
}

void showNormalMessage() {
    std::cout << "All vitals are within normal limits." << std::endl;
}

// -----------------------------
// Main Application Flow
// -----------------------------

std::vector<std::string> checkAllVitals(
    const std::vector<VitalReading>& readings,
    const VitalChecker& checker
) {
    std::vector<std::string> alerts;

    for (const auto& reading : readings) {
        auto alertOpt = checker.check(reading);
        if (alertOpt.has_value()) {
            alerts.push_back(alertOpt.value());
        }
    }

    return alerts;
}

// -----------------------------
// Main Program
// -----------------------------

int main() {
    std::vector<VitalReading> readings = {
        {"Temperature", 98.6f},
        {"PulseRate", 110.0f},  // Out of range
        {"SpO2", 95.0f},
        {"BloodPressure", 120.0f}  // Unknown
    };

    VitalLimitsRepository limitsRepo;
    VitalChecker checker(limitsRepo);

    auto alerts = checkAllVitals(readings, checker);

    if (alerts.empty()) {
        showNormalMessage();
    } else {
        for (const auto& alert : alerts) {
            showAlert(alert);
        }
    }

    return 0;
}
