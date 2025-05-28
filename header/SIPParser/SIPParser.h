#pragma once

#include <cstdio>
#include <cstring>
#include <cassert>
#include <string>
#include <queue>

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

private:
  SIPStrMessages& m_internalMessages;
  SIPStrMessages& m_invalidMessages;
  SIPParsedMessages& m_unmodifiedMessages;
};

