#pragma once

#include <queue>

//#include <sofia-sip/sip.h>

#include <utils/types.h>

class SIPManipulator{
public:
  SIPManipulator() = delete;
  SIPManipulator(SIPParsedMessages &unmodifiedMessages,
                 SIPParsedMessages &modifiedMessages)
      : m_unmodifiedMessages(unmodifiedMessages),
        m_modifiedMessages(modifiedMessages) {}

  void Proceed()
  {
    std::swap(m_unmodifiedMessages, m_modifiedMessages);
  }

private:
  SIPParsedMessages& m_unmodifiedMessages;
  SIPParsedMessages& m_modifiedMessages;
};
