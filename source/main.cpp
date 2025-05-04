#include <SIPParser/SIPParser.h>
#include <transport/UDP_transport.h>

int main() {
  std::queue<std::string> internalMessages;
  std::queue<std::string> externalMessages;
  std::queue<std::string> invalidMessages;
  std::queue<sip_t*> unmodifiedMessages;

  UDP_transport udp_transport(
      {{{"127.0.0.2", 5060}}, {"127.0.0.3", 5060}},
      internalMessages, externalMessages);
  SIPParser sipParser(internalMessages, invalidMessages, unmodifiedMessages);

  try {
    while (true) {
      udp_transport.Proceed();
      sipParser.Proceed();
      while(!unmodifiedMessages.empty()){
        auto sip = unmodifiedMessages.front();
        unmodifiedMessages.pop();
        printf("Call-ID: %s\n", sip->sip_call_id->i_id);
        printf("Method: %s\n", sip->sip_request->rq_method_name);
        printf("From: %s\n", sip->sip_from->a_url->url_user);
      }
    }
  } catch (...) {
    std::cout << "Error: " << std::endl;
    return EXIT_FAILURE;
  }
}
