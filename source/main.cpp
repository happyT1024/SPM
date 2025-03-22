#include <transport/UDP_transport.h>

int main() {
  Config config{{{"127.0.0.1", 8080}, {"127.0.0.1", 8081}},
                {"192.168.0.2", 9090}};
  UDP_transport udp_transport(std::move(config));

  try {
    while (true) {
      udp_transport.Proceed();
      std::swap(udp_transport.internalPackets, udp_transport.externalPackets);
    }
  } catch (...) {
    std::cout << "Error: " << std::endl;
    return EXIT_FAILURE;
  }
}
