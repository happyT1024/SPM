#pragma once

#include <config/Config.h>

#include <arpa/inet.h>
#include <bits/stdc++.h>
#include <cstdlib>
#include <cstring>
#include <netinet/in.h>
#include <queue>
#include <utility>
#include <sys/socket.h>

class UDP_transport {
public:
  using SIPPackets = std::queue<std::string>;

public:
  UDP_transport() = delete;

  explicit UDP_transport(Config config)
      : m_config(std::move(config))
  {
    init();
  }

  void Proceed() {
    while (!externalPackets.empty()) {
      sendto(m_externalSock, externalPackets.front().c_str(),
             externalPackets.front().length(), MSG_NOSIGNAL,
             reinterpret_cast<const sockaddr *>(&m_externalAddr),
             sizeof(m_externalAddr));
      std::cout << "Отправлено сообщение: " << externalPackets.front() << std::endl;
      externalPackets.pop();
    }
    // TODO: epoll
    for (int i = 0; i < m_internalSocks.size(); ++i) {
      auto &sock = m_internalSocks[i];
      auto &addr = m_internalAddrs[i];
      char buff[1000];
      socklen_t addrlen = sizeof(addr);
      int nbytes = recvfrom(sock, buff, 1000, 0,
                            reinterpret_cast<sockaddr *>(&addr), &addrlen);
      if (nbytes == -1) {
        perror("Ошибка при получении данных");
      }
      std::string packet(buff, nbytes);
      std::cout << "Получено сообщение: " << packet << std::endl;
      internalPackets.push(packet);
    }
  }

  ~UDP_transport() {
    close(m_externalSock);
    for (auto &sock : m_internalSocks) {
      close(sock);
    }
  }

  SIPPackets internalPackets;
  SIPPackets externalPackets;

private:
  void init() {
    const auto &gateToAddr = [&](const Gate &gate) {
      sockaddr_in addr = {};
      addr.sin_family = AF_INET;
      addr.sin_port = htons(gate.m_port);
      inet_pton(AF_INET, gate.m_host.c_str(), &addr.sin_addr);
      return addr;
    };

    const auto &getSock = []() {
      int sock;
      if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
      }
      return sock;
    };

    const auto &bindSock = [](int sock, const sockaddr_in &addr) {
      socklen_t addrlen = sizeof(addr);
      if (bind(sock, reinterpret_cast<const sockaddr *>(&addr), addrlen) < 0) {
        perror("socket bind failed");
        exit(EXIT_FAILURE);
      }
    };

    m_internalSocks.resize(m_config.m_internalGates.size());
    for (int i = 0; i < m_config.m_internalGates.size(); ++i) {
      m_internalAddrs.emplace_back(gateToAddr(m_config.m_internalGates[i]));
      m_internalSocks[i] = getSock();
      bindSock(m_internalSocks[i], m_internalAddrs[i]);
    }
    m_externalAddr = gateToAddr(m_config.m_externalGate);
    m_externalSock = getSock();
    bindSock(m_externalSock, m_externalAddr);
  }

private:
  Config m_config;

  int m_externalSock{};
  std::vector<int> m_internalSocks;

  sockaddr_in m_externalAddr{};
  std::vector<sockaddr_in> m_internalAddrs;
};
