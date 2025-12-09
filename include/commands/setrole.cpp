#include "pch.hpp"
#include "tools/string.hpp"
#include "setrole.hpp"

void setrole(ENetEvent& event, const std::string_view message)
{
    auto &peer = _peer[event.peer];
    if (peer->user_id != 1) return; // @note only for the owner/id 1

    std::vector<std::string> args = readch(std::string(message), ' '); // /setrole <name> <role>
    if (args.size() < 3) 
    {
        packet::create(*event.peer, false, 0, { "OnConsoleMessage", "Usage: /setrole <name> <role_id (1=Mod, 2=Dev)>" });
        return;
    }

    std::string target_name = args[1];
    int role_id = std::stoi(args[2]);

    bool found = false;
    for (auto& p : _peer) 
    {
        if (p.second->ltoken[0] == target_name) 
        {
            p.second->role = static_cast<u_char>(role_id);
            found = true;
            
            packet::create(*event.peer, false, 0, { "OnConsoleMessage", std::format("Successfully set role of {} to {}. They must re-login.", target_name, role_id) });
            packet::create(*p.first, false, 0, { "OnConsoleMessage", std::format("Your role has been updated to {}. Please re-enter the world to see effects.", role_id) });
            break; 
        }
    }

    if (!found)
    {
         packet::create(*event.peer, false, 0, { "OnConsoleMessage", "Player not found online (Must be online to set role currently)." });
    }
}
