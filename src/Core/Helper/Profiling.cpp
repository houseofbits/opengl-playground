#include "Profiling.h"

std::vector<Profiling::Profile> Profiling::profiles = [] {
    std::vector<Profile> v;
    v.resize(200);
    return v;
}();
