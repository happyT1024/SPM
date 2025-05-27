#pragma once

#include <queue>
#include <string>

#include <sofia-sip/sip.h>

using SIPStrMessages = std::queue<std::string>;
using SIPParsedMessages = std::queue<msg_t*>;
