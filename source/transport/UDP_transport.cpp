#include <transport/UDP_transport.h>

UDP_transport::UDP_transport(Config config, SIPPackets &internalPackets,
                             SIPPackets &externalPackets)
    : m_config(std::move(config)), m_internalPackets(internalPackets),
      m_externalPackets(externalPackets) {
  init();
}

void UDP_transport::Proceed() {
  while (!m_externalPackets.empty()) {
    sendto(m_externalSock, m_externalPackets.front().c_str(),
           m_externalPackets.front().length(), MSG_NOSIGNAL,
           reinterpret_cast<const sockaddr *>(&m_externalAddr),
           sizeof(m_externalAddr));
    std::cout << "Отправлено сообщение: " << m_externalPackets.front()
              << std::endl;
    m_externalPackets.pop();
  }

  epoll_event events[MAX_EVENTS];
  int nfds = epoll_wait(m_epollfd, events, MAX_EVENTS, 1000);
  for (int n = 0; n < nfds; ++n) {
    if (events[n].events & EPOLLIN) {
      char buf[1024];
      ssize_t count = read(events[n].data.fd, buf, sizeof(buf));
      if (count == -1) {
        perror("read");
      } else if (count > 0) {
        //printf("Read %zd bytes: %.*s\n", count, (int)count, buf);
      }
      std::string packet(buf, count);
      m_internalPackets.push(packet);
    }
  }
}

UDP_transport::~UDP_transport() {
  close(m_externalSock);
  for (auto &sock : m_internalSocks) {
    close(sock);
  }
  close(m_epollfd);
}

void UDP_transport::init() {
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

  const auto &addToEpoll = [this](int sock) {
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
}
