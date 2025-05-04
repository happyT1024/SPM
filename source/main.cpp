#include <SIPParser/SIPParser.h>
#include <transport/UDP_transport.h>
#include <session/SessionLayer.h>

int main() {
  std::queue<std::string> internalMessages;
  std::queue<std::string> externalMessages;
  std::queue<std::string> invalidMessages;
  std::queue<sip_t*> unmodifiedMessages;
  std::queue<sip_t*> modifiedMessages;

  UDP_transport udp_transport(
      {{{"127.0.0.2", 5060}}, {"127.0.0.3", 5060}},
      internalMessages, externalMessages);
  SIPParser sipParser(internalMessages, invalidMessages, unmodifiedMessages);
  SessionLayer sessionLayer(unmodifiedMessages, modifiedMessages);

  try {
    while (true) {
      udp_transport.Proceed();
      sipParser.Proceed();
      sessionLayer.Proceed();

    }
  } catch (...) {
    std::cout << "Error: " << std::endl;
    return EXIT_FAILURE;
  }
}
