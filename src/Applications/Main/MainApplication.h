#pragma once

#include "../../Core/API.h"

class MainApplication final : public Application {
public:
    MainApplication();

    ~MainApplication() override;

    void run() override;

    void initialize(const std::string &fileName, const std::string &entityDefinitionFileName);
};
