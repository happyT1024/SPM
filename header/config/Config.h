#pragma once

#include <config/Gate.h>

#include <vector>

class Config{
public:
  using Gates = std::vector<Gate>;
public:
  Config() = delete;

  Config(Gates internalGates, const Gate& externalGate)
      : m_internalGates(std::move(internalGates))
        , m_externalGate(externalGate)
  {}

  Gates m_internalGates;
  Gate m_externalGate;
};
