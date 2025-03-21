#include <transport/UDP_transport.h>

int main() {
  Config config{{{"127.0.0.1", 8080}, {"127.0.0.1", 8081}},
                {"127.0.0.1", 8082}};
  UDP_transport udp_transport(std::move(config));

  try {
    while (true) {
      udp_transport.Proceed();
      if (!udp_transport.internalPackets.empty()) {
        std::swap(udp_transport.internalPackets, udp_transport.externalPackets);
      }
      else {
        sleep(1);
      }
    }
  } catch (...) {
    std::cout << "Error: " << std::endl;
    return EXIT_FAILURE;
  }
}
