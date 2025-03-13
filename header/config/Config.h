#pragma once

#include <config/Gate.h>

#include <vector>

class Config{
public:
  using Gates = std::vector<Gate>;

  Gates m_internalGates;
  Gate m_externalGate;
};
