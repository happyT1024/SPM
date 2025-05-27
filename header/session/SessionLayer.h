#pragma once

#include <queue>

#include <sofia-sip/sip.h>

#include <utils/types.h>

class SessionLayer{
public:
  SessionLayer() = delete;
  SessionLayer(SIPParsedMessages &modifiedMessages,
               SIPStrMessages &externalMessages,
               const Gate& externalGate)
      : m_modifiedMessages(modifiedMessages),
        m_externalMessages(externalMessages),
        m_externalGate(externalGate){}

  void Proceed()
  {
    while(!m_modifiedMessages.empty())
    {
      msg_t* msg = m_modifiedMessages.front();

      // Буфер для хранения результата
      //char buffer[4096];  // Достаточно большой для SIP-сообщения
      // Сериализуем (mo = msg_pub_t*)
      void ** buffer = NULL;
      int bytes_read = msg_recv_buffer(msg, buffer);
      msg_serialize;
      // int len = msg_recv_buffer(msg, reinterpret_cast<void **>(buffer));
      // m_externalMessages.emplace(buffer);
      msg_destroy(msg);
      std::cout<<bytes_read<<'\n';
      m_modifiedMessages.pop();
    }
  }

private:
  SIPParsedMessages& m_modifiedMessages;
  SIPStrMessages& m_externalMessages;
  Gate m_externalGate;
};
