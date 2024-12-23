#include "ContinuousSystemProcess.h"

ContinuousSystemProcess::ContinuousSystemProcess(EventManager &eventManager, long frequencyMs): BaseSystemProcess(
        eventManager),
    m_thread(&ContinuousSystemProcess::process, this),
    m_processRunning(true),
    m_frequencyMs(frequencyMs) {
}

void ContinuousSystemProcess::process() {
    while (true) {
        if (!m_processRunning) {
            return;
        }
        auto start = std::chrono::steady_clock::now();

        processAllSystems();

        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::steady_clock::now() - start).count();

        if (elapsed < m_frequencyMs) {
            std::this_thread::sleep_for(std::chrono::milliseconds(m_frequencyMs - elapsed));
        }
    }
}
