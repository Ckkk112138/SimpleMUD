#ifndef LOGON_H
#define LOGON_H

#include "SocketLib.h"
#include "SimpleMUDLogs.h"
#include <string>

using SocketLib::Telnet;
using SocketLib::Connection;
using std::string;

namespace SimpleMUD
{
    enum LogonState
    {
        NEWCONNECTION,           // first state
        NEWUSER,                 // new user, enter desired name
        ENTERNEWPASS,            // new user, enter desired password
        ENTERPASS                // existing user, enter password
    };
}
#endif // !LOGON_H

