#include "SimpleMUDLogs.h"

namespace SimpleMUD
{
	BasicLib::TextLog ERRORLOG("logs/errors.log", "Error Log", true, true);
	BasicLib::TextLog USERLOG("logs/users.log", "User Log", true, true);

}