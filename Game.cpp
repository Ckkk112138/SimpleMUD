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
            //Logout(p.ID());
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
        if (firstword == "kick" && p.Rank() >= GOD) {
            PlayerDatabase::iterator itr =
                PlayerDatabase::findloggedin(ParseWord(p_data, 1));
            if (itr == PlayerDatabase::end()) {
                p.SendString(red + bold + "Player could not be found.");
                return;
            }
            if (itr->Rank() > p.Rank()) {
                p.SendString(red + bold + "You can't kick that player!");
                return;
            }
            itr->Conn()->Close();
            LogoutMessage(itr->Name() + " has been kicked by " +
                p.Name() + "!!!");
            PlayerDatabase::Logout(itr->ID());
            return;
        }
        if (firstword == "announce" && p.Rank() >= ADMIN) {
            Announce(RemoveWord(p_data, 0));
            return;
        }

        if (firstword == "changerank" && p.Rank() >= ADMIN) {
            string name = ParseWord(p_data, 1);
            PlayerDatabase::iterator itr = PlayerDatabase::find(name);
            if (itr == PlayerDatabase::end()) {
                p.SendString(red + bold + "Error: Could not find user " +
                    name);
                return;
            }
            PlayerRank rank = GetRank(ParseWord(p_data, 2));
            itr->Rank() = rank;
            SendGame(green + bold + itr->Name() +
                "'s rank has been changed to: " +
                GetRankString(rank));
            return;
        }
        if (firstword == "reload" && p.Rank() >= ADMIN) {
            string db = BasicLib::LowerCase(ParseWord(p_data, 1));
            if (db == "items") {
                ItemDatabase::Load();
                p.SendString(bold + cyan + "Item Database Reloaded!");
            }
        }
        if (firstword == "shutdown" && p.Rank() >= ADMIN) {
            Announce("SYSTEM IS SHUTTING DOWN");
            Game::Running() = false;
            return;
        }
        SendGame(bold + p.Name() + " chats: " + p_data);

    }

    void Game::SendGlobal(const string& p_str)
    {
        operate_on_if(PlayerDatabase::begin(), PlayerDatabase::end(), playersend(p_str), playerloggedin());
    }

    void Game::SendGame(const string& p_str)
    {
        operate_on_if(PlayerDatabase::begin(), PlayerDatabase::end(), playersend(p_str), playeractive());
    }

    void Game::LogoutMessage(const string& p_reason)
    {
        SendGame(red + bold + p_reason);
    }

    void Game::Announce(const string& p_announcement)
    {
        SendGlobal(cyan + bold + "Attention to all players: " + p_announcement);
    }

    void Game::Whisper(string p_str, string p_player)
    {
        PlayerDatabase::iterator itr = PlayerDatabase::findactive(p_player);
        if (itr == PlayerDatabase::end())
        {
            m_player->SendString(red + bold + "Connot find user.");
        }
        else
        {
            itr->SendString(yellow + m_player->Name() + " whispers to you: " +
                reset + p_str);
            m_player->SendString(yellow + "You whisper to " + itr->Name() + ": " + reset + p_str);
        }
    }

    struct wholist
    {
        string str;
        void operator()(Player& p)
        {
            str += " " + tostring(p.Name()) + "| ";
            str += tostring(p.Level()) + "| ";


            if (p.Active())
                str += green + "Online  " + white;
            else if (p.LoggedIn())
                str += yellow + "Inactive" + white;
            else
                str += red + "Offline " + white;
            str += " | ";
            switch (p.Rank())
            {
            case REGULAR:   str += white;   break;
            case GOD:       str += yellow;  break;
            case ADMIN:     str += green;   break;
            }
            str += GetRankString(p.Rank());
            str += white + "rn";
        }
        
    };

    string Game::WhoList(const string& p_who)
    {
        using namespace BasicLib;
        string str = white + bold +
            "--------------------------------------------------------------------------------\r\n" +
            " Name             | Level     | Activity | Rank\r\n" +
            "--------------------------------------------------------------------------------\r\n";
        wholist who;
        if (p_who == "all")
        {
            who = BasicLib::operate_on_if(
                PlayerDatabase::begin(),
                PlayerDatabase::end(),
                wholist(),
                True<Player>());
        }
        else
        {
            who = BasicLib::operate_on_if(
                PlayerDatabase::begin(),
                PlayerDatabase::end(),
                wholist(),
                playerloggedin());
        }
        str += who.str;

        str +=
            "--------------------------------------------------------------------------------";
        return str;
    }

    bool Game::UseItem(const string& p_item)
    {
        Player& p = *m_player;
        int i = p.GetItemIndex(p_item);
        if (i == -1)
        {
            p.SendString(red + bold + "Could not find that item!");
            return false;
        }
        Item& itm = *p.GetItem(i);
        switch (itm.Type())
        {
        case WEAPON:
            p.UseWeapon(i);
            return true;
        case ARMOR:
            p.UseArmor(i);
            return true;
        case HEALING:
            p.AddBonuses(itm.ID());
            p.AddHP(itm.Min());
            p.DropItem(i);
            return true;
        }
        return false;
    }

    bool Game::RemoveItem(string p_item)
    {
        Player& p = *m_player;
        p_item = LowerCase(p_item);
        if (p_item == "weapon" && p.Weapon() != 0)
        {
            p.RemoveWeapon();
            return true;
        }
        if (p_item == "armor" && p.Armor() != 0)
        {
            p.RemoveWeapon();
            return true;
        }
        p.SendString(red + bold + "You don't have a item to remove!");
        return false;
    }

    void Game::GotoTrain()
    {
        Player& p = *m_player;
        p.Active() = false;
        p.Conn()->AddHandler(new Train(*m_connection,m_player));
        LogoutMessage(p.Name() + " leaves to edit stats.");
    }

    string Game::PrintHelp(PlayerRank p_rank)
    {
        static string help = white + bold +
            "--------------------------------- Command List ---------------------------------\r\n" +
            " /                          - Repeats your last command exactly.\r\n" +
            " chat <mesg>                - Sends message to everyone in the game\r\n" +
            " experience                 - Shows your experience statistics\r\n" +
            " help                       - Shows this menu\r\n" +
            " inventory                  - Shows a list of your items\r\n" +
            " quit                       - Allows you to leave the realm.\r\n" +
            " remove <'weapon'/'armor'>  - removes your weapon or armor\r\n" +
            " stats                      - Shows all of your statistics\r\n" +
            " time                       - shows the current system time.\r\n" +
            " use <item>                 - use an item in your inventory\r\n" +
            " whisper <who> <msg>        - Sends message to one person\r\n" +
            " who                        - Shows a list of everyone online\r\n" +
            " who all                    - Shows a list of everyone\r\n" +
            " look                       - Shows you the contents of a room\r\n" +
            " north/east/south/west      - Moves in a direction\r\n" +
            " get/drop <item>            - Picks up or drops an item on the ground\r\n" +
            " train                      - Train to the next level (TR)\r\n" +
            " editstats                  - Edit your statistics (TR)\r\n" +
            " list                       - Lists items in a store (ST)\r\n" +
            " buy/sell <item>            - Buy or Sell an item in a store (ST)\r\n" +
            " attack <enemy>             - Attack an enemy\r\n";
        static string god = yellow + bold +
            "--------------------------------- God Commands ---------------------------------\r\n" +
            " kick <who>                 - kicks a user from the realmrn";
        static string admin = green + bold +
            "-------------------------------- Admin Commands --------------------------------\r\n" +
            " announce <msg>             - Makes a global system announcement\r\n" +
            " changerank <who> <rank>    - Changes the rank of a player\r\n" +
            " reload <db>                - Reloads the requested database\r\n" +
            " shutdown                   - Shuts the server down\r\n";
        static string end = white + bold +
            "--------------------------------------------------------------------------------";
        if (p_rank == REGULAR)
            return help + end;
        else if (p_rank == GOD)
            return help + god + end;
        else if (p_rank == ADMIN)
            return help + god + admin + end;
        else return "ERROR";
    }

    string Game::PrintStats()
    {
        using namespace BasicLib;
        Player& p = *m_player;
        return white + bold +
            "---------------------------------- Your Stats ----------------------------------\r\n" +
            " Name:          " + p.Name() + "\r\n" +
            " Rank:          " + GetRankString(p.Rank()) + "\r\n" +
            " HP/Max:        " + tostring(p.HitPoints()) + "/" + tostring(p.GetAttr(MAXHITPOINTS)) +
            "  (" + tostring(percent(p.HitPoints(), p.GetAttr(MAXHITPOINTS))) + "%)\r\n" +
            PrintExperience() + "\r\n" +
            " Strength:      " + tostring(p.GetAttr(STRENGTH), 16) +
            " Accuracy:      " + tostring(p.GetAttr(ACCURACY)) + "\r\n" +
            " Health:        " + tostring(p.GetAttr(HEALTH), 16) +
            " Dodging:       " + tostring(p.GetAttr(DODGING)) + "\r\n" +
            " Agility:       " + tostring(p.GetAttr(AGILITY), 16) +
            " Strike Damage: " + tostring(p.GetAttr(STRIKEDAMAGE)) + "\r\n" +
            " StatPoints:    " + tostring(p.StatPoints(), 16) +
            " Damage Absorb: " + tostring(p.GetAttr(DAMAGEABSORB)) + "\r\n" +
            "--------------------------------------------------------------------------------";
    }
 
    string Game::PrintExperience()
    {
        using namespace BasicLib;
        Player& p = *m_player;
        return white + bold +
            " Level:         " + tostring(p.Level()) + "\r\n" +
            " Experience:    " + tostring(p.Experience()) + "/" +
            tostring(p.NeedforLevel(p.Level() + 1)) + " (" +
            tostring(percent(p.Experience(), p.NeedforLevel(p.Level() + 1))) +
            "%)";
    }

    string Game::PrintInventory()
    {
        using namespace BasicLib;
        Player& p = *m_player;
        // Inventory
        string itemlist = white + bold +
            "-------------------------------- Your Inventory --------------------------------\r\n" +
            " Items:  ";
        for (int i = 0; i < PLAYERITEMS; i++)
        {
            if (p.GetItem(i) != 0)
            {
                itemlist += p.GetItem(i)->Name() + ", ";
            }
        }
        // chop off the extraneous comma, and add a newline.
        itemlist.erase(itemlist.size() - 2, 2);
        itemlist += "\r\n";
        // Weapon/Armor
        itemlist += " Weapon: ";
        if (p.Weapon() == 0)
            itemlist += "NONE!";
        else
            itemlist += p.Weapon()->Name();
        itemlist += "\r\n Armor:  ";
        if (p.Armor() == 0)
            itemlist += "NONE!";
        else
            itemlist += p.Armor()->Name();
        // Money
        itemlist += "\r\n Money:  $" + tostring(p.Money());
        itemlist +=
            "\r\n--------------------------------------------------------------------------------";

        return itemlist;
    }
}