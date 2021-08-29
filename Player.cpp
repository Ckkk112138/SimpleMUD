#include <cmath>
#include "Player.h"
#include "SimpleMUDLogs.h"

namespace SimpleMUD
{
	Player::Player()
	{
        m_password = "UNDEFINED";
        m_rank = REGULAR;
        m_connection = nullptr;
        m_loggedin = false;
        m_active = false;
        m_newbie = true;
        m_experience = 0;
        m_level = 1;
        m_room = 1;
        m_money = 0;
        m_nextattacktime = 0;
        m_baseattributes[STRENGTH] = 1;
        m_baseattributes[HEALTH] = 1;
        m_baseattributes[AGILITY] = 1;
        m_items = 0;
        m_weapon = -1;
        m_armor = -1;
        m_statpoints = 18;
        RecalculateStats();
        m_hp = GetAttr(MAXHITPOINTS);
	}

    int Player::NeedForNextLevel()
    {
        return NeedforLevel(m_level + 1) - m_experience;
    }

    bool Player::Train()
    {
        if (NeedForNextLevel() <= 0)
        {
            m_statpoints += 2;
            m_baseattributes[MAXHITPOINTS] += m_level;
            m_level++;
            RecalculateStats();
            return true;
        }
        else {
            return false;
        }
    }

    void Player::RecalculateStats()
    {
        m_attributes[MAXHITPOINTS] = 10 + ((int)(m_level * (GetAttr(HEALTH) / 1.5)));
        m_attributes[HPREGEN] = (GetAttr(HEALTH) / 5) + m_level;
        m_attributes[ACCURACY] = GetAttr(AGILITY) * 3;
        m_attributes[DODGING] = GetAttr(AGILITY) * 3;
        m_attributes[DAMAGEABSORB] = GetAttr(STRENGTH) / 5;
        m_attributes[STRIKEDAMAGE] = GetAttr(STRENGTH) / 5;
        // make sure the hitpoints don't overflow if your max goes down
        if (m_hp > GetAttr(MAXHITPOINTS))
            m_hp = GetAttr(MAXHITPOINTS);

        if (Weapon() != 0)
            AddDynamicBonuses(Weapon());
        if (Armor() != 0)
            AddDynamicBonuses(Armor());
    }

    void Player::AddDynamicBonuses(item p_item)
    {
        if (p_item == nullptr)
            return;
        Item& i = *p_item;
        for (int j = 0; j < NUMATTRIBUTES; ++j)
        {
            m_attributes[j] += i.GetAttr(j);
        }
    }

    void Player::SetBaseAttr(int p_attr, int p_val)
    {
        m_baseattributes[p_attr] = p_val;
        RecalculateStats();
    }

    void Player::AddToBaseAttr(int p_attr, int p_val)
    {
        m_baseattributes[p_attr] += p_val;
        RecalculateStats();
    }

    void Player::SetHP(int p_hp)
    {
        m_hp = p_hp;
        if (m_hp < 0)
            m_hp = 0;
        if (m_hp > GetAttr(MAXHITPOINTS))
            m_hp = GetAttr(MAXHITPOINTS);
    }

    void Player::AddHP(int p_hp)
    {
        SetHP(m_hp + p_hp);
    }

    bool Player::PickUpItem(item p_item)
    {
        if (m_items < MaxItems())
        {
            item* itr = m_inventory;
            while (*itr != nullptr)
                ++itr;
            *itr = p_item;
            m_items++;
            return true;
        }
        return false;
    }

    bool Player::DropItem(int p_index)
    {
        if (m_inventory[p_index] != nullptr)
        {
            if (m_weapon == p_index)
                RemoveWeapon();
            if (m_armor == p_index)
                RemoveArmor();
            m_inventory[p_index] = 0;
            m_items--;
            return true;
        }
        return false;
    }

    void Player::AddBonuses(item p_item)
    {
        if (p_item == nullptr)
            return;
        Item& itm = *p_item;
        for (int i = 0; i < NUMATTRIBUTES; ++i)
        {
            m_baseattributes[i] += itm.GetAttr(i);
        }
        RecalculateStats();
    }

    void Player::RemoveWeapon()
    {
        m_weapon = -1;
        RecalculateStats();
    }

    void Player::RemoveArmor()
    {
        m_armor = -1;
        RecalculateStats();
    }

    void Player::UseWeapon(int p_index)
    {
        RemoveWeapon();
        m_weapon = p_index;
        RecalculateStats();
    }

    void Player::UseArmor(int p_index)
    {
        RemoveArmor();
        m_armor = p_index;
        RecalculateStats();
    }

    int Player::GetItemIndex(const string& p_name)
    {
        item* i = double_find_if(m_inventory, m_inventory + MaxItems(), matchentityfull(p_name), matchentity(p_name));
        if (i == m_inventory + MaxItems())
            return -1;
        return (int)(i - m_inventory);
    }

    void Player::SendString(const string& p_str)
    {
        using namespace SocketLib;
        if (Conn() == nullptr)
        {
            ERRORLOG.Log("Trying to send to player" +
                Name() + "but player is not connected");
            return;
        }
        Conn()->Protocol().SendString(*Conn(), p_str + newline);
        if (Active())
            PrintStasbar();
    }

    void Player::PrintStasbar(bool p_update)
    {
        using namespace SocketLib;
        if (p_update && Conn()->Protocol().Buffered() > 0)
            return;

        string statbar = white + bold + "[";
        int ratio = 100 * HitPoints() / GetAttr(MAXHITPOINTS);
        if (ratio < 33)
            statbar += red;
        else if (ratio < 67)
        {
            statbar = yellow;
        }
        else
            statbar += green;
        statbar += tostring(HitPoints()) + white + "/" + tostring(GetAttr(MAXHITPOINTS)) + "]";
        Conn()->Protocol().SendString(*Conn(), clearline + "r" + statbar + reset);
    }

}