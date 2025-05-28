#include <SIPParser/SIPParser.h>

void SIPParser::Proceed()
{
  while(!m_internalMessages.empty())
  {
    auto& str = m_internalMessages.front();
    m_unmodifiedMessages.emplace(str);
    m_internalMessages.pop();
  }
}
