#pragma once

#include "../../Core/API.h"

class MainApplication final : public Application {
public:
    MainApplication();

    void run() override;

    void initialize(const std::string &entityDefinitionFileName);
};
