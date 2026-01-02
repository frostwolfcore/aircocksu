#pragma once

#include "isteamclient.h"
#include "isteamapplist.h"	
#include "isteamfriends.h"
#include "isteamcontroller.h"
#include "isteamgamecoordinator.h"
#include "isteamgameserver.h"
#include "isteamgameserverstats.h"
#include "isteamhtmlsurface.h"
#include "isteamhttp.h"
#include "isteaminventory.h"
#include "steamtypes.h"
#include "steamclientpublic.h"
#include "isteamuser.h"
#include "isteamuserstats.h"
#include "isteamutils.h"
#include "isteamnetworking.h"
#include "isteamugc.h"


class c_steam_sdk
{
public:
	void initialize_local();
};

extern c_steam_sdk g_steam_sdk;