/**
 * vim: set ts=4 :
 * =============================================================================
 * SourceMod Navigation File Download Blocker
 * Copyright (C) 2012.  All rights reserved.
 * =============================================================================
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License, version 3.0, as published by the
 * Free Software Foundation.
 * 
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * As a special exception, AlliedModders LLC gives you permission to link the
 * code of this program (as well as its derivative works) to "Half-Life 2," the
 * "Source Engine," the "SourcePawn JIT," and any Game MODs that run on software
 * by the Valve Corporation.  You must obey the GNU General Public License in
 * all respects for all other code used.  Additionally, AlliedModders LLC grants
 * this exception to all derivative works.  AlliedModders LLC defines further
 * exceptions, found in LICENSE.txt (as of this writing, version JULY-31-2007),
 * or <http://www.sourcemod.net/license.php>.
 *
 * Version: $Id$
 */

#include "extension.h"

NavFileBlocker g_instance;
SMEXT_LINK(&g_instance);

CDetour *g_pOnResourcePrecachedFullPath = NULL;
IGameConfig *g_pGameConf = NULL;

bool NavFile(char *file);

DETOUR_DECL_MEMBER1(OnResourcePrecachedFullPath, int, char *, file_name)
{
	if (NavFile(file_name))
	{
		return 0;
	}

	return DETOUR_MEMBER_CALL(OnResourcePrecachedFullPath)(file_name);
}

bool NavFileBlocker::SDK_OnLoad(char *error, size_t maxlength, bool late)
{
	char gamedata_error[255] = "";
	if (!gameconfs->LoadGameConfigFile("navfile-blocker", &g_pGameConf, gamedata_error, sizeof(gamedata_error)))
	{
		if (gamedata_error[0])
		{
			snprintf(error, maxlength, "Could not read navfile-blocker.txt: %s", gamedata_error);
		}
		return false;
	}

	CDetourManager::Init(g_pSM->GetScriptingEngine(), g_pGameConf);

	g_pOnResourcePrecachedFullPath = DETOUR_CREATE_MEMBER(OnResourcePrecachedFullPath, "OnResourcePrecachedFullPath");
	if (g_pOnResourcePrecachedFullPath)
	{
		g_pOnResourcePrecachedFullPath->EnableDetour();
	} 
	else 
	{
		snprintf(error, maxlength, "Could not create the detour.");
		return false;
	}

	return true;
}

void NavFileBlocker::SDK_OnUnload()
{
	if (g_pOnResourcePrecachedFullPath)
	{
		g_pOnResourcePrecachedFullPath->Destroy();
	}

	gameconfs->CloseGameConfigFile(g_pGameConf);
}

bool NavFile(char *file)
{
	size_t len = strlen(file);
	
	if (len > 3 && stricmp(file + len - 4, ".nav") == 0)
	{
		return true;
	}

	/*if (strstr(file, ".nav") != NULL)
	{
		return true;
	}*/

	return false;
}