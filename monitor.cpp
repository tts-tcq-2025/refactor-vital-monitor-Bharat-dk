#include <thread>
#include <chrono>
#include <iostream>

using std::cout, std::flush, std::this_thread::sleep_for, std::chrono::seconds;

// Function to show blinking alert
void showAlert(const char* message) {
  cout << message << "\n";
  for (int i = 0; i < 6; ++i) {
    cout << "\r* " << flush;
    sleep_for(seconds(1));
    cout << "\r *" << flush;
    sleep_for(seconds(1));
  }
}

// Generic range checker
bool isOutOfRange(float value, float lowerLimit, float upperLimit, bool inclusive = false) {
  return inclusive ? (value < lowerLimit || value > upperLimit)
                   : (value <= lowerLimit || value >= upperLimit);
}

// Refactored vitalsOk function with further reduced complexity
int vitalsOk(float temperature, float pulseRate, float spo2) {
  struct VitalCheck {
    float value;
    float low;
    float high;
    const char* message;
  };

  VitalCheck checks[] = {
    { temperature, 95.0f, 102.0f, "Temperature is critical!" },
    { pulseRate, 60.0f, 100.0f, "Pulse Rate is out of range!" },
    { spo2, 90.0f, 200.0f, "Oxygen Saturation out of range!" }  // Upper bound set high as only low limit matters
  };

  for (const auto& check : checks) {
    if (isOutOfRange(check.value, check.low, check.high, true)) {
      showAlert(check.message);
      return 0;
    }
  }

  return 1;
}

int main() {
  vitalsOk(98, 110, 98);  // Example call
}
