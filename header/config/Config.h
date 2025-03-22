#pragma once

#include <config/Gate.h>

#include <vector>

class Config {
public:
  using Gates = std::vector<Gate>;

public:
  Config() = delete;
  Config(Gates internalGates, Gate externalGate)
      : m_internalGates(std::move(internalGates)),
        m_externalGate(std::move(externalGate)) {}
  Config(const Config &other) = default;
  Config &operator=(const Config &other) = default;
  Config(Config &&other) = default;
  Config &operator=(Config &&other) = default;

  Gates m_internalGates;
  Gate m_externalGate;
};
