#include <iostream>
#include <vector>
#include <string>
#include <functional>

// Represents a vital sign limit and name
struct VitalSign {
    std::string name;
    float lowLimit;
    float highLimit;
    bool inclusive;
};

// Represents a vital reading with its name and value
struct VitalReading {
    std::string name;
    float value;
};

// Pure function to check if value is out of range
bool isOutOfRange(float value, float low, float high, bool inclusive = false) {
    if (inclusive)
        return value < low || value > high;
    else
        return value <= low || value >= high;
}

// Pure function that returns vector of alert messages for vitals out of range
std::vector<std::string> checkVitals(const std::vector<VitalReading>& readings,
                                     const std::vector<VitalSign>& limits) {
    std::vector<std::string> alerts;

    for (const auto& limit : limits) {
        auto it = std::find_if(readings.begin(), readings.end(),
                               [&](const VitalReading& r) { return r.name == limit.name; });
        if (it != readings.end()) {
            if (isOutOfRange(it->value, limit.lowLimit, limit.highLimit, limit.inclusive)) {
                alerts.push_back(limit.name + " is out of range!");
            }
        } else {
            alerts.push_back("No reading for " + limit.name);
        }
    }

    return alerts;
}

// Alert function (I/O side-effect separated)
void showAlert(const std::string& message) {
    std::cout << message << "\n";
    // Blinking alert logic could be here if desired
}

int main() {
    // Define limits
    std::vector<VitalSign> limits = {
        {"Temperature", 95.0f, 102.0f, true},
        {"PulseRate", 60.0f, 100.0f, true},
        {"SpO2", 90.0f, 200.0f, true}  // Upper bound high for oxygen saturation
    };

    // Example readings (could come from sensors or vendors)
    std::vector<VitalReading> readings = {
        {"Temperature", 98.0f},
        {"PulseRate", 110.0f},
        {"SpO2", 98.0f}
    };

    // Check vitals
    auto alerts = checkVitals(readings, limits);

    if (!alerts.empty()) {
        for (const auto& alert : alerts) {
            showAlert(alert);
        }
    } else {
        std::cout << "All vitals are normal.\n";
    }

    return 0;
}
