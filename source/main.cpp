#include <SIPParser/SIPParser.h>
#include <transport/UDP_transport.h>
#include <session/SessionLayer.h>
#include <Manipulator/SIPManipulator.h>
#include <session/SessionLayer.h>
#include <utils/types.h>

int main() {
  SIPStrMessages internalMessages;
  SIPStrMessages externalMessages;
  SIPStrMessages invalidMessages;
  SIPParsedMessages unmodifiedMessages;
  SIPParsedMessages modifiedMessages;

  Config config{{{"127.0.0.2", 5060}}, {"127.0.0.3", 5060}};

  UDP_transport udp_transport(
      config,
      internalMessages, externalMessages);
  SIPParser sipParser(internalMessages, invalidMessages, unmodifiedMessages);
  SIPManipulator sipManipulator(unmodifiedMessages, modifiedMessages);
  SessionLayer sessionLayer(modifiedMessages, externalMessages, config.m_externalGate);

  try {
    while (true) {
      udp_transport.Proceed();
      sipParser.Proceed();
      sipManipulator.Proceed();
      sessionLayer.Proceed();
    }
  } catch (...) {
    std::cout << "Error: " << std::endl;
    return EXIT_FAILURE;
  }
}
