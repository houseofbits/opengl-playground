#pragma once
#include "EntitySystem.h"

class EntitySystemRegistry {
private:
    class SystemsProcess {
    public:
        SystemsProcess(const long frequencyMs, EventManager &eventManager) : m_eventManager(&eventManager),
                                                                             m_thread(&SystemsProcess::process, this),
                                                                             m_processRunning(true),
                                                                             m_frequencyMs(frequencyMs) {
        }

        ~SystemsProcess() {
            m_processRunning = false;
            m_thread.join();
        }

        void process() const {
            while (true) {
                if (!m_processRunning) {
                    return;
                }
                auto start = std::chrono::steady_clock::now();

                for (const auto &system: m_Systems) {
                    system->process(*m_eventManager);
                }

                auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
                    std::chrono::steady_clock::now() - start).count();

                if (elapsed < m_frequencyMs) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(m_frequencyMs - elapsed));
                }
            }
        }

        EventManager *m_eventManager;
        std::list<EntitySystem *> m_Systems;
        std::thread m_thread;
        std::atomic<bool> m_processRunning;
        long m_frequencyMs;
    };

public:
    EntitySystemRegistry();

    void initialize(EventManager &eventManager) {
        m_physicsSystemProcess = new SystemsProcess(100, eventManager);
    }

    SystemsProcess *m_physicsSystemProcess;
};
