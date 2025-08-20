#include <iostream>
#include <string>
#include <vector>
#include <functional>

// Vital limits structure
struct VitalLimits {
    float low;
    float high;
    bool inclusive;
};

// Pure functions to check individual vitals
bool checkTemperature(float temp, const VitalLimits& limits) {
    if (limits.inclusive)
        return temp < limits.low || temp > limits.high;
    else
        return temp <= limits.low || temp >= limits.high;
}

bool checkPulseRate(float pulse, const VitalLimits& limits) {
    if (limits.inclusive)
        return pulse < limits.low || pulse > limits.high;
    else
        return pulse <= limits.low || pulse >= limits.high;
}

bool checkSpO2(float spo2, const VitalLimits& limits) {
    if (limits.inclusive)
        return spo2 < limits.low || spo2 > limits.high;
    else
        return spo2 <= limits.low || spo2 >= limits.high;
}

// Alert function (I/O separated)
void showAlert(const std::string& message) {
    std::cout << message << std::endl;
}

// Main function to evaluate vitals with minimal complexity per function
int evaluateVitals(float temperature, float pulseRate, float spo2) {
    // Define limits - could be moved to config or data file
    const VitalLimits tempLimits{95.0f, 102.0f, true};
    const VitalLimits pulseLimits{60.0f, 100.0f, true};
    const VitalLimits spo2Limits{90.0f, 200.0f, true};  // high upper bound

    if (checkTemperature(temperature, tempLimits)) {
        showAlert("Temperature is critical!");
        return 0;
    }

    if (checkPulseRate(pulseRate, pulseLimits)) {
        showAlert("Pulse Rate is out of range!");
        return 0;
    }

    if (checkSpO2(spo2, spo2Limits)) {
        showAlert("Oxygen Saturation is out of range!");
        return 0;
    }

    return 1;  // All vitals okay
}

// Simple test cases for coverage completeness
void runTests() {
    // Test data: value, limits, expected result
    struct TestCase {
        float value;
        VitalLimits limits;
        bool expected;
    };

    // Temperature tests
    TestCase tempTests[] = {
        {94.9f, {95.0f, 102.0f, true}, true},
        {95.0f, {95.0f, 102.0f, true}, false},
        {102.0f, {95.0f, 102.0f, true}, false},
        {102.1f, {95.0f, 102.0f, true}, true},
    };

    for (auto& test : tempTests) {
        bool res = checkTemperature(test.value, test.limits);
        if (res != test.expected) {
            std::cerr << "Temperature test failed for value " << test.value << std::endl;
        }
    }

    // Pulse rate tests (similar pattern)
    TestCase pulseTests[] = {
        {59.9f, {60.0f, 100.0f, true}, true},
        {60.0f, {60.0f, 100.0f, true}, false},
        {100.0f, {60.0f, 100.0f, true}, false},
        {100.1f, {60.0f, 100.0f, true}, true},
    };

    for (auto& test : pulseTests) {
        bool res = checkPulseRate(test.value, test.limits);
        if (res != test.expected) {
            std::cerr << "PulseRate test failed for value " << test.value << std::endl;
        }
    }

    // SpO2 tests
    TestCase spo2Tests[] = {
        {89.9f, {90.0f, 200.0f, true}, true},
        {90.0f, {90.0f, 200.0f, true}, false},
        {199.9f, {90.0f, 200.0f, true}, false},
        {200.0f, {90.0f, 200.0f, true}, false},
        {200.1f, {90.0f, 200.0f, true}, true},
    };

    for (auto& test : spo2Tests) {
        bool res = checkSpO2(test.value, test.limits);
        if (res != test.expected) {
            std::cerr << "SpO2 test failed for value " << test.value << std::endl;
        }
    }

    std::cout << "Tests completed\n";
}

int main() {
    runTests();

    // Example usage
    int status = evaluateVitals(98.0f, 110.0f, 98.0f);
    if (status == 1) {
        std::cout << "All vitals normal." << std::endl;
    }
    return 0;
}
