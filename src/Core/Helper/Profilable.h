#pragma once

#include <cassert>
#include <chrono>
#include <string>
#include "Log.h"

class Profilable {
public:
    Profilable();

    void beginProfiling() {
        previousStart = std::chrono::high_resolution_clock::now();
        isOn = true;
    }

    void endProfiling() {
        if (!isOn) {
            return;
        }

        const std::chrono::duration<double, std::milli> duration = std::chrono::high_resolution_clock::now()
                                                                   - previousStart;;
        numSamples++;
        totalDurationMs += duration.count();
    }

    [[nodiscard]] double getProfilingTime() const {
        if (numSamples == 0) {
            return 0;
        }

        return totalDurationMs / numSamples;
    }

private:
    std::chrono::high_resolution_clock::time_point previousStart;
    unsigned int numSamples{0};
    double totalDurationMs{0};
    bool isOn{false};
};
