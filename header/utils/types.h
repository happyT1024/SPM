#pragma once

#include <queue>
#include <string>

#include <SIPParser/SIP_Message.h>


using SIPStrMessages = std::queue<std::string>;
using SIPParsedMessages = std::queue<Ds_SIP_Message>;
