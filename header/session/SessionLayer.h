#pragma once

#include <queue>

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
      m_externalMessages.push(m_modifiedMessages.front().to_str());
      m_modifiedMessages.pop();
    }
  }

private:
  SIPParsedMessages& m_modifiedMessages;
  SIPStrMessages& m_externalMessages;
  Gate m_externalGate;
};
