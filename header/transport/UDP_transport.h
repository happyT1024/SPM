#pragma once

#include <config/Config.h>

#include <queue>
#include <arpa/inet.h>
#include <bits/stdc++.h>
#include <cstdlib>
#include <cstring>
#include <netinet/in.h>
#include <sys/socket.h>

class UDP_transport {
public:
  using SIPPackets = std::queue<std::string>;
  UDP_transport(const Config & config)
      : m_config(config)
  {}

  void Proceed(){
    while(!externalPacket.empty())
    {
      sendto(externalSock, externalPacket.front().c_str(), externalPacket.front().length(), MSG_NOSIGNAL,
             reinterpret_cast<const sockaddr *>(&externalAddr), sizeof(externalAddr));
      externalPacket.pop();
    }
    for()
  }

  SIPPackets internalPacket;
  SIPPackets externalPacket;

private:
  Config m_config;
  int externalSock;
  sockaddr_in externalAddr;
};