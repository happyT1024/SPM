#pragma once

#include <config/Config.h>

#include <arpa/inet.h>
#include <bits/stdc++.h>
#include <cstdlib>
#include <cstring>
#include <netinet/in.h>
#include <queue>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <utility>

#define MAX_EVENTS 10

class UDP_transport {
public:
  using SIPPackets = std::queue<std::string>;

public:
  UDP_transport() = delete;

  UDP_transport(Config config, SIPPackets& internalPackets, SIPPackets& externalPackets);

  void Proceed();

  ~UDP_transport();

  SIPPackets& m_internalPackets;
  SIPPackets& m_externalPackets;

private:
  void init();

private:
  // TODO: хз зачем хранить конфигу полностью, потом выпилить или создать
  // возможность перезагрузить конфиг
  Config m_config;

  int m_externalSock{};
  std::vector<int> m_internalSocks;

  sockaddr_in m_externalAddr{};
  std::vector<sockaddr_in> m_internalAddrs;

  int m_epollfd{};
};
