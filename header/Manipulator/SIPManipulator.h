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
    while(!m_unmodifiedMessages.empty())
    {
      auto& msg = m_unmodifiedMessages.front();
      msg.from.name_address = "Matvey <sip:matveyCorrectURI@spbsut.com>";
      m_modifiedMessages.push(msg);
      m_unmodifiedMessages.pop();
    }
  }

  void LoadRule(const std::string& rule){}

private:
  SIPParsedMessages& m_unmodifiedMessages;
  SIPParsedMessages& m_modifiedMessages;
  std::string path;
  std::string value;
};
