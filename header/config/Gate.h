#pragma once

#include <string>

class Gate{
public:
  Gate() = delete;
  Gate(const std::string& host, int port)
  : m_host(host)
  , m_port(port)
  {}
  Gate(const Gate& other) = default;
  Gate& operator=(const Gate& other) = default;

  std::string m_host;
  int m_port;
};