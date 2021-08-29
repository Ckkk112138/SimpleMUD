#include "Game.h"

namespace SimpleMUD
{
    Timer Game::s_timer;
    bool Game::isRunning = false;

	void Game::Enter()
	{
        USERLOG.Log(GetIPString(m_connection->GetRemoteAddress()) +
            " - User " + m_player->Name() +
            " entering Game state.");
        m_lastcommand = "";
        Player& p = *m_player;
        p.Active() = true;
        p.LoggedIn() = true;
        SendGame(bold + green + p.Name() + " has entered the realm.");
        if (p.Newbie())
            GotoTrain();

	}

    void Game::Leave()
    {
        m_player->Active() = false;
        if (m_connection->Closed())
            PlayerDatabase::Logout(m_player);
    }

    void Game::Hungup()
    {
        Player& p = *m_player;
        LogoutMessage(p.Name() + " has suddenly disappeared from the realm.");
    }

    void Game::Flooded()
    {
        Player& p = *m_player;
        LogoutMessage(p.Name() + " has been kicked out for flooding!.");
    }

    void Game::Handle(string p_data)
    {
        Player& p = *m_player;
        if (p_data == "/")
            p_data = m_lastcommand;
        else
        {
            m_lastcommand = p_data;
        }
        string firstword = LowerCase(ParseWord(p_data, 0));

        if (firstword == "chat" || firstword == ":")
        {
            string text = RemoveWord(p_data, 0);
            SendGame(white + bold + p.Name() + " chats: " + text);
            return;
        }
        if (firstword == "experience" || firstword == "exp") {
            p.SendString(PrintExperience());
            return;
        }
        if (firstword == "help" || firstword == "commands") {
            p.SendString(PrintHelp(p.Rank()));
            return;
        }
        if (firstword == "inventory" || firstword == "i") {
            p.SendString(PrintInventory());
            return;
        }
        if (firstword == "stats" || firstword == "st") {
            p.SendString(PrintStats());
            return;
        }
        if (firstword == "quit")
        {
            m_connection->Close();
            LogoutMessage(p.Name() + " has left the realm.");
            Logout(p.ID());
            return;
        }
        if (firstword == "remove") {
            RemoveItem(ParseWord(p_data, 1));
            return;
        }
        if (firstword == "use") {
            UseItem(RemoveWord(p_data, 0));
            return;
        }
        if (firstword == "time") {
            p.SendString(bold + cyan +
                "The current system time is: " + TimeStamp() +
                " on " + DateStamp() +
                "\r\nThe system has been up for: "
                + s_timer.GetString() + ".");
            return;
        }
        if (firstword == "whisper")
        {
            string name = ParseWord(p_data, 0);
            string msg = RemoveWord(RemoveWord(p_data, 0), 0);
            Whisper(msg, name);
            return;
        }
        if (firstword == "who")
        {
            p.SendString(WhoList(LowerCase(ParseWord(p_data, 1))));
            return;
        }

    }


}