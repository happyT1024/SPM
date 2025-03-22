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
#include <sys/epoll.h>

#define MAX_EVENTS 10

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

    epoll_event events[MAX_EVENTS];
    int nfds = epoll_wait(m_epollfd, events, MAX_EVENTS, 100);
    for (int n = 0; n < nfds; ++n) {
      if (events[n].events & EPOLLIN) {
        char buf[1024];
        ssize_t count = read(events[n].data.fd, buf, sizeof(buf));
        if (count == -1) {
          perror("read");
        } else if (count > 0) {
          printf("Read %zd bytes: %.*s\n", count, (int)count, buf);
        }
        std::string packet(buf, count);
        internalPackets.push(packet);
      }
    }
  }

  ~UDP_transport() {
    close(m_externalSock);
    for (auto &sock : m_internalSocks) {
      close(sock);
    }
    close(m_epollfd);
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

    const auto& addToEpoll = [this](int sock) {
      epoll_event ev{};
      ev.events = EPOLLIN;
      ev.data.fd = sock;
      if (epoll_ctl(m_epollfd, EPOLL_CTL_ADD, sock, &ev) == -1) {
        perror("epoll_ctl: sock");
        exit(EXIT_FAILURE);
      }
    };

    const auto pollSize = static_cast<int>(m_config.m_internalGates.size());

    m_internalSocks.resize(pollSize);
    m_epollfd = epoll_create(pollSize);

    if (m_epollfd == -1) {
      perror("epoll_create");
      exit(EXIT_FAILURE);
    }

    for (int i = 0; i < m_config.m_internalGates.size(); ++i) {
      m_internalAddrs.emplace_back(gateToAddr(m_config.m_internalGates[i]));
      m_internalSocks[i] = getSock();
      bindSock(m_internalSocks[i], m_internalAddrs[i]);
      addToEpoll(m_internalSocks[i]);
    }
    m_externalAddr = gateToAddr(m_config.m_externalGate);
    m_externalSock = getSock();
    bindSock(m_externalSock, m_externalAddr);
    addToEpoll(m_externalSock);
  }

private:
  // TODO: хз зачем хранить конфигу полностью, потом выпилить или создать возможность перезагрузить конфиг
  Config m_config;

  int m_externalSock{};
  std::vector<int> m_internalSocks;

  sockaddr_in m_externalAddr{};
  std::vector<sockaddr_in> m_internalAddrs;

  int m_epollfd;
};
