#pragma once
#include "packetcreator.h"
#include "winapp.h"
#include "settings.h"

using namespace program;
using namespace net;

extern packetcreator packet_c;
extern winapp app;
extern session server;
extern settings config;

extern int result;

extern void quit();
