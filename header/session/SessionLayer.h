#pragma once

#include <queue>

#include <sofia-sip/sip.h>

class SessionLayer{
public:
  SessionLayer() = delete;
  SessionLayer(std::queue<sip_t *> &unmodifiedMessages,
               std::queue<sip_t *> &modifiedMessages)
      : m_unmodifiedMessages(unmodifiedMessages),
        m_modifiedMessages(modifiedMessages) {}

  void Proceed()
  {
    std::swap(m_unmodifiedMessages, m_modifiedMessages);
  }

private:
  std::queue<sip_t*>& m_unmodifiedMessages;
  std::queue<sip_t*>& m_modifiedMessages;
};
