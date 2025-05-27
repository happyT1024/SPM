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

#include <utils/types.h>

class SIPParser{
public:
  SIPParser() = delete;
  SIPParser(SIPStrMessages& internalMessages,
            SIPStrMessages& invalidMessages,
            SIPParsedMessages& unmodifiedMessages)
      : m_internalMessages(internalMessages)
      , m_invalidMessages(invalidMessages)
      , m_unmodifiedMessages(unmodifiedMessages)
  {}

  void Proceed();

private:
  static msg_t* parse_memory(msg_mclass_t const *mclass, char const data[], int len);

private:
  SIPStrMessages& m_internalMessages;
  SIPStrMessages& m_invalidMessages;
  SIPParsedMessages& m_unmodifiedMessages;
  msg_mclass_t const * mclass = sip_default_mclass();  // Получаем стандартный класс сообщений SIP
};

