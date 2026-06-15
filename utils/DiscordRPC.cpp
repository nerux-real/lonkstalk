#include "DiscordRPC.h"
#include <iostream>

void DiscordRPC::init(const char* appId){
    DiscordEventHandlers handlers = {};
    Discord_Initialize(appId, &handlers, 1, nullptr);
    std::cout << "Discord RPC initialized\n";
}

void DiscordRPC::update(const std::string& state, const std::string& details, const std::string& largeImageKey){
    DiscordRichPresence presence = {};
    presence.state   = state.c_str();
    presence.details = details.c_str();
    if(!largeImageKey.empty())
        presence.largeImageKey = largeImageKey.c_str();
    presence.startTimestamp = time(nullptr);
    Discord_UpdatePresence(&presence);
}

void DiscordRPC::shutdown(){
    Discord_Shutdown();
}

void DiscordRPC::runCallbacks(){
    Discord_RunCallbacks();
}