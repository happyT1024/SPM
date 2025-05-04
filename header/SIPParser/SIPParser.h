#pragma once

#include <cstdio>
#include <cstring>
#include <cassert>
#include <string>
#include <queue>

#include <sofia-sip/sip.h>
#include <sofia-sip/msg_parser.h>
#include <sofia-sip/msg.h>
#include <sofia-sip/su.h>
#include <sofia-sip/msg_addr.h>
#include <sofia-sip/sip_parser.h>


class SIPParser{
public:
  SIPParser() = delete;
  SIPParser(std::queue<std::string>& internalMessages,
            std::queue<std::string>& invalidMessages,
            std::queue<sip_t*>& unmodifiedMessages)
      : m_internalMessages(internalMessages)
      , m_invalidMessages(invalidMessages)
      , m_unmodifiedMessages(unmodifiedMessages)
  {}

  void Proceed();

private:
  static msg_t* parse_memory(msg_mclass_t const *mclass, char const data[], int len);

private:
  std::queue<std::string>& m_internalMessages;
  std::queue<std::string>& m_invalidMessages;
  std::queue<sip_t*>& m_unmodifiedMessages;
  msg_mclass_t const * mclass = sip_default_mclass();  // Получаем стандартный класс сообщений SIP
};

