#include <SIPParser/SIPParser.h>

void SIPParser::Proceed()
{
  auto strMsg = m_internalMessages.front().c_str();
  m_internalMessages.pop();
  int len = static_cast<int>(strlen(strMsg));
  msg_t* msg = parse_memory(mclass, strMsg, len);
  sip_t* sip = sip_object(msg);
  if (!sip) {
    printf("Неверный SIP-пакет!\n");
    m_invalidMessages.emplace(strMsg);
    return;
  }
  m_unmodifiedMessages.emplace(sip);
}

msg_t* SIPParser::parse_memory(msg_mclass_t const *mclass, char const data[], int len)
{
  msg_t *msg;
  int m;
  msg_iovec_t iovec[2] = {{ 0 }};
  int n = len;

  msg  = msg_create(mclass, 0);
  if (!msg)
    return NULL;

  m = msg_recv_iovec(msg, iovec, 2, n, 1);
  if (m < 0) {
    msg_destroy(msg);
    return NULL;
  }
  assert(m <= 2);
  assert(iovec[0].mv_len + iovec[1].mv_len == n);

  memcpy(iovec[0].mv_base, data, n = iovec[0].mv_len);
  if (m == 2)
    memcpy((iovec[1].mv_base + n), data + n, iovec[1].mv_len);

  msg_recv_commit(msg, iovec[0].mv_len + iovec[1].mv_len, 1);

  m = msg_extract(msg);
  assert(m != 0);
  if (m < 0) {
    msg_destroy(msg);
    return NULL;
  }
  return msg;
}