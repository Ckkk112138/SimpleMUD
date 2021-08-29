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

    class Logon :public Telnet::handler
    {
    protected:
        LogonState m_state;
        int m_errors;
        string m_name;
        string m_password;
    public:
        Logon(Connection<Telnet>& p_conn)
            :Telnet::handler(p_conn)
        {
            m_state = NEWCONNECTION;
            m_errors = 0;
        }

        void Handle(string p_data);
        void Enter();
        void Leave() {}
        void Hungup()
        {
            USERLOG.Log(SocketLib::GetIPString(m_connection->GetRemoteAddress()) +
                " - hung up in login state.");
        }

        void Flooded()
        {
            USERLOG.Log(SocketLib::GetIPString(m_connection->GetRemoteAddress()) +
                " - flooded in login state.");
        }

        void GotoGame(bool p_newbie = false);

        static void NoRoom(Connection<Telnet>& p_conn)
        {
            static string msg = "Sorry, there is no more room on this server.\r\n";
            try
            {
                p_conn.Send(msg.c_str(), (int)msg.size());
            }
            catch (SocketLib::Exception)
            {

            }
        }

        static bool AcceptibleName(const string& p_name);

    };
}
#endif // !LOGON_H

