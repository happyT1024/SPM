#pragma once

#include <string>
#include <utility>

class Gate {
public:
  Gate() = delete;
  Gate(std::string host, int port) : m_host(std::move(host)), m_port(port) {}
  Gate(const Gate &other) = default;
  Gate &operator=(const Gate &other) = default;
  Gate(Gate &&other) = default;
  Gate &operator=(Gate &&other) = default;

  std::string m_host;
  int m_port;
};