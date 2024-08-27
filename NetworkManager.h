#pragma once
#include "config.h"

class NetworkManager {
    static bool addSocket(SOCKET id, int what);
    static void removeSocket(int index);
    static void acceptConnection(int index);
    static void receiveMessage(int index);
    static void sendMessage(int index);
};
