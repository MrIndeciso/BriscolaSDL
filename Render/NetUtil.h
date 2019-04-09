#ifndef NETUTIL_H_INCLUDED
#define NETUTIL_H_INCLUDED

#include "GameUtil.h"
#include "../Lib/SDL/include/SDL_net.h"

int initServer();

int initClient();

int localConnect();

int sendMano();

int receiveMano();

#endif
