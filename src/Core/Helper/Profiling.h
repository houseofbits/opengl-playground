#pragma once

#include <cassert>
#include <vector>
#include <chrono>
#include <string>
#include "Log.h"

class Profiling {
public:
    struct Profile {
        std::string name;
        std::chrono::high_resolution_clock::time_point previousStart;
        unsigned int numSamples{0};
        double totalDurationMs{0};
        bool isOn{false};
    };

    static void begin(const unsigned int key) {
        assert(key < 200);

        profiles[key].previousStart = std::chrono::high_resolution_clock::now();
        profiles[key].isOn = true;
    }

    static void end(const unsigned int key) {
        assert(key < 200);

        if (!profiles[key].isOn) {
            return;
        }

        const std::chrono::duration<double, std::milli> duration = std::chrono::high_resolution_clock::now()
                                                                   - profiles[key].previousStart;;
        profiles[key].numSamples++;
        profiles[key].totalDurationMs += duration.count();
    }

    static void endToEnd(const unsigned int key) {
        end(key);
        begin(key);
    }

    static void setName(const unsigned int key, const std::string &name) {
        assert(key < 200);

        profiles[key].name = name;
    }

    static void print() {
        Log::write("Profile");
        for (auto &profile: profiles) {
            if (profile.numSamples > 0) {
                auto ts = profile.totalDurationMs / profile.numSamples;
                Log::write(profile.name, " ", ts);
            }
        }
    }

    static std::vector<Profile> profiles;
};
