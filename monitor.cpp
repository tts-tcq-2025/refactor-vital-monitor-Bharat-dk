#include <iostream>
#include <string>
#include <vector>
#include <optional>

// Data structure to hold vital limits
struct VitalLimits {
    float low;
    float high;
    bool inclusive;  // true means strict inequality (<, >)
};

// Data structure for a vital sign reading
struct VitalReading {
    std::string name;
    float value;
};

// Pure function to check a single value against limits with CCN = 2
bool isOutOfRange(float value, const VitalLimits& limits) {
    if (limits.inclusive) {
        return value < limits.low || value > limits.high;
    }
    return value <= limits.low || value >= limits.high;
}

// Function to get alert message for a specific vital (pure, CCN=1)
std::optional<std::string> checkVital(const VitalReading& reading, const VitalLimits& limits) {
    if (isOutOfRange(reading.value, limits)) {
        return reading.name + " is out of range!";
    }
    return std::nullopt;
}

// Function to find limits for a vital by name, CCN=1 (no branching)
std::optional<VitalLimits> getLimitsForVital(const std::string& name) {
    if (name == "Temperature") return VitalLimits{95.0f, 102.0f, true};
    if (name == "PulseRate") return VitalLimits{60.0f, 100.0f, true};
    if (name == "SpO2") return VitalLimits{90.0f, 200.0f, true};
    return std::nullopt;  // Unknown vital
}

// Main function to check all vitals, CCN=3 max (one for loop + two early returns)
std::vector<std::string> checkAllVitals(const std::vector<VitalReading>& readings) {
    std::vector<std::string> alerts;

    for (const auto& reading : readings) {
        auto limitsOpt = getLimitsForVital(reading.name);
        if (!limitsOpt.has_value()) {
            alerts.push_back("Unknown vital: " + reading.name);
            continue;
        }
        auto alertOpt = checkVital(reading, limitsOpt.value());
        if (alertOpt.has_value()) {
            alerts.push_back(alertOpt.value());
        }
    }

    return alerts;
}

// I/O separated function to show alert
void showAlert(const std::string& message) {
    std::cout << message << std::endl;
}

// Sample main program
int main() {
    // Example vital readings (could come from sensors or vendors)
    std::vector<VitalReading> readings = {
        {"Temperature", 98.6f},
        {"PulseRate", 110.0f},  // Out of range
        {"SpO2", 95.0f}
    };

    auto alerts = checkAllVitals(readings);

    if (alerts.empty()) {
        std::cout << "All vitals are within normal limits." << std::endl;
    } else {
        for (const auto& alert : alerts) {
            showAlert(alert);
        }
    }

    return 0;
}
