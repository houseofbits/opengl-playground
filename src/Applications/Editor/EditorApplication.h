#pragma once

#include "../../Core/API.h"

class EditorApplication : public Application {
public:
  EditorApplication();

  ~EditorApplication() override;

  void run() override;

  void initialize(const std::string &FileName,
                  const std::string &entityDefinitionFileName);
};
