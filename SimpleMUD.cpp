// SimpleMUD.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "ItemDatabase.h"
#include "PlayerDatabase.h"
#include "SocketLib.h"
#include "Game.h"
#include "SimpleMUDLogs.h"
#include "ThreadLib.h"
#include "Entity.h"

using namespace SocketLib;
using namespace SimpleMUD;


using namespace std;

struct match
{
	std::string m_str;
	match(const std::string& p_str)
		:m_str(p_str)
	{

	}
	bool operator()(pair<int,string> par)
	{
		return par.second == m_str;
	}
	

};

int main()
{
	try
	{
		PlayerDatabase::Load();
		PlayerDatabase::PrintPlayer();
		ItemDatabase::Load();
		
		
		ListeningManager<Telnet, Logon> lm;
		ConnectionManager<Telnet, Logon> cm(128, 60, 65536);

		lm.SetConnectionManager(&cm);
		lm.AddPort(5000);
		Game::GetTimer().Reset();
		Game::Running() = true;
		while (Game::Running())
		{
			lm.Listen();
			cm.Manage();
			ThreadLib::YieldThread();
		}
			

	}
	catch (SocketLib::Exception& e) {   // catch socket exceptions
		ERRORLOG.Log("Fatal Socket Error: " + e.PrintError());
	}
	catch (std::exception& e) {         // catch standard exceptions
		ERRORLOG.Log("Standard Error: " + std::string(e.what()));
	}
	catch (...) {                       // catch other exceptions
		ERRORLOG.Log("Unspecified Error");
	}
	PlayerDatabase::Save();   // save the player database


	/*map<int, string> m;
	m[0] = "ckw";
	m[1] = "cll";
	map<int, string>::iterator itr = find_if(m.begin(), m.end(), match("ckw"));
	cout << itr->second;*/
    
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
