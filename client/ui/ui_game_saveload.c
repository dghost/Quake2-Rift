/*
Copyright (C) 1997-2001 Id Software, Inc.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/

// ui_game_saveload.c -- the single save/load menus

#include <ctype.h>
#ifdef _WIN32
#include <io.h>
#endif
#include "../client.h"
#include "include/ui_local.h"
#include <sys/types.h>
#include <sys/stat.h>

#define	MAX_SAVEGAMES	21 // was 15

static menuframework_s	s_loadgame_menu;
static menuaction_s		s_loadgame_actions[MAX_SAVEGAMES];
static menuaction_s		s_loadgame_back_action;

static menuframework_s	s_savegame_menu;
static menuaction_s		s_savegame_actions[MAX_SAVEGAMES];
static menuaction_s		s_savegame_back_action;


/*
=============================================================================

SAVESHOT HANDLING

=============================================================================
*/

char		m_savestrings[MAX_SAVEGAMES][32];
qboolean	m_savevalid[MAX_SAVEGAMES];
time_t		m_savetimestamps[MAX_SAVEGAMES];
qboolean	m_savechanged[MAX_SAVEGAMES];
struct image_s *m_saveshots[MAX_SAVEGAMES+1];

char		m_mapname[MAX_QPATH];
//qboolean	m_mapshotvalid;

void Load_Savestrings (qboolean update)
{
	int32_t		i;
	FILE	*fp;
	fileHandle_t	f;
	char	name[MAX_OSPATH];
	char	mapname[MAX_TOKEN_CHARS];
	char	*ch;
	time_t	old_timestamp;
	struct	stat	st;
    const char *gamedir = FS_Gamedir();
	for (i=0 ; i<MAX_SAVEGAMES ; i++)
	{
		Com_sprintf (name, sizeof(name), "%s/save/vrsave%02i/server.ssv", gamedir, i);

		old_timestamp = m_savetimestamps[i];
        if (stat(name, &st) == 0) {
            m_savetimestamps[i] = st.st_mtime;
        } else {
            m_savetimestamps[i] = 0;
        }

        m_savechanged[i] = !(update && m_savetimestamps[i] == old_timestamp);
		// doesn't need to be refreshed
        if ( !m_savechanged[i] ) {
			continue;
		}

		fp = fopen (name, "rb");
		if (!fp) {
		//	Com_Printf("Save file %s not found.\n", name);
			strcpy (m_savestrings[i], "<EMPTY>");
			m_savevalid[i] = false;
			m_savetimestamps[i] = 0;
		}
		else
		{
			fclose (fp);
			Com_sprintf (name, sizeof(name), "save/vrsave%02i/server.ssv", i);
			FS_FOpenFile (name, &f, FS_READ);
			if (!f)
			{
				//Com_Printf("Save file %s not found.\n", name);
				strcpy (m_savestrings[i], "<EMPTY>");
				m_savevalid[i] = false;
				m_savetimestamps[i] = 0;
			}
			else
			{
				FS_Read (m_savestrings[i], sizeof(m_savestrings[i]), f);

				if (i==0) { // grab mapname
					FS_Read (mapname, sizeof(mapname), f);
					if (mapname[0] == '*') // skip * marker
						Com_sprintf (m_mapname, sizeof(m_mapname), mapname+1);
					else
						Com_sprintf (m_mapname, sizeof(m_mapname), mapname);
					if ((ch = strchr (m_mapname, '$')))
						*ch = 0; // terminate string at $ marker
				}
				FS_FCloseFile(f);
				m_savevalid[i] = true;
			}
		}
	}
}

void ValidateSaveshots (void)
{
	int32_t i;
	char shotname [MAX_QPATH];
//	char mapshotname [MAX_QPATH];

	for ( i = 0; i < MAX_SAVEGAMES; i++ )
	{
		if ( !m_savechanged[i] )	// doeesn't need to be reloaded
			continue;
		if ( m_savevalid[i] )
		{
			if (i == 0)
				Com_sprintf(shotname, sizeof(shotname), "/levelshots/%s.pcx", m_mapname);
			else
			{	// free previously loaded shots
				Com_sprintf(shotname, sizeof(shotname), "save/vrsave%02i/shot.png", i);
				R_FreePic (shotname);
				Com_sprintf(shotname, sizeof(shotname), "/save/vrsave%02i/shot.png", i);
			}
            m_saveshots[i] = R_DrawFindPic(shotname);
		}
		else
            m_saveshots[i] = NULL;
	}
}

void UI_UpdateSavegameData (void)
{
	Load_Savestrings (true);
	ValidateSaveshots ();	// register saveshots
}

void UI_InitSavegameData (void)
{
	int32_t		i;

	for (i=0; i<MAX_SAVEGAMES; i++) {
		m_savetimestamps[i] = 0;
		m_savechanged[i] = true;
	}

	Load_Savestrings (false);
	ValidateSaveshots ();	// register saveshots

	// register null saveshot, this is only done once
    m_saveshots[MAX_SAVEGAMES] = R_DrawFindPic(UI_NOSCREEN_NAME);
}


void DrawSaveshot (qboolean loadmenu)
{
	int32_t i;
	
	if (loadmenu)
	{
		if (s_loadgame_menu.cursor +1  == s_loadgame_menu.nitems)
			return;
		i = s_loadgame_actions[s_loadgame_menu.cursor].generic.localdata[0];
	}
	else
	{
		if (s_savegame_menu.cursor +1 == s_savegame_menu.nitems)
			return;
		i = s_savegame_actions[s_savegame_menu.cursor].generic.localdata[0];
	}
    
	SCR_DrawFill (SCREEN_WIDTH/2+44, SCREEN_HEIGHT/2-60, 244, 184, ALIGN_CENTER, 60,60,60,255);

	if (m_savevalid[i] && m_saveshots[i])
	{
		SCR_DrawImage (SCREEN_WIDTH/2+46, SCREEN_HEIGHT/2-58, 240, 180, ALIGN_CENTER, m_saveshots[i], 1.0);
    }
	else if (m_saveshots[MAX_SAVEGAMES])
		SCR_DrawImage(SCREEN_WIDTH/2+46, SCREEN_HEIGHT/2-58, 240, 180, ALIGN_CENTER, m_saveshots[MAX_SAVEGAMES], 1.0);
	else
		SCR_DrawFill (SCREEN_WIDTH/2+46, SCREEN_HEIGHT/2-58, 240, 180, ALIGN_CENTER, 0,0,0,255);

}


/*
=============================================================================

LOADGAME MENU

=============================================================================
*/

struct image_s *load_saveshot;
char loadshotname[MAX_QPATH];

void LoadGameCallback( void *self )
{
	menuaction_s *a = ( menuaction_s * ) self;

	// set saveshot name here
	if ( m_saveshots[ a->generic.localdata[0] ] ) {
		Com_sprintf(loadshotname, sizeof(loadshotname), "/save/vrsave%02i/shot.png", a->generic.localdata[0]);
		load_saveshot = R_DrawFindPic(loadshotname) ;
    } else
		load_saveshot = NULL;

	if ( m_savevalid[ a->generic.localdata[0] ] )
		Cbuf_AddText (va("load vrsave%02i\n",  a->generic.localdata[0] ) );
	UI_ForceMenuOff ();
}

void LoadGame_MenuInit ( void )
{
	int32_t i;

	UI_UpdateSavegameData ();

	s_loadgame_menu.x = SCREEN_WIDTH*0.5 - 240;
	s_loadgame_menu.y = SCREEN_HEIGHT*0.5 - 58;
	s_loadgame_menu.nitems = 0;

//	Load_Savestrings ();

	for ( i = 0; i < MAX_SAVEGAMES; i++ )
	{
		s_loadgame_actions[i].generic.name			= m_savestrings[i];
		s_loadgame_actions[i].generic.flags			= QMF_LEFT_JUSTIFY;
		s_loadgame_actions[i].generic.localdata[0]	= i;
		s_loadgame_actions[i].generic.callback		= LoadGameCallback;

		s_loadgame_actions[i].generic.x = 0;
		s_loadgame_actions[i].generic.y = (i) * MENU_LINE_SIZE;
		if (i>0)	// separate from autosave
			s_loadgame_actions[i].generic.y += 10;

		s_loadgame_actions[i].generic.type = MTYPE_ACTION;

		Menu_AddItem( &s_loadgame_menu, &s_loadgame_actions[i] );
	}

	s_loadgame_back_action.generic.type	= MTYPE_ACTION;
	s_loadgame_back_action.generic.flags  = QMF_LEFT_JUSTIFY;
	s_loadgame_back_action.generic.x		= 0;
	s_loadgame_back_action.generic.y		= (MAX_SAVEGAMES+3)*MENU_LINE_SIZE;
	s_loadgame_back_action.generic.name		= " back";
	s_loadgame_back_action.generic.callback = UI_BackMenu;

	Menu_AddItem( &s_loadgame_menu, &s_loadgame_back_action );
}

void LoadGame_MenuDraw( void )
{
	Menu_DrawBanner( "m_banner_load_game" );
//	Menu_AdjustCursor( &s_loadgame_menu, 1 );
	Menu_Draw( &s_loadgame_menu );
	DrawSaveshot (true);
}

const char *LoadGame_MenuKey( int32_t key )
{
	if ( key == K_ESCAPE || key == K_ENTER )
	{
		s_savegame_menu.cursor = s_loadgame_menu.cursor - 1;
		if ( s_savegame_menu.cursor < 0 )
			s_savegame_menu.cursor = 0;
	}
	return Default_MenuKey( &s_loadgame_menu, key );
}

void M_Menu_LoadGame_f (void)
{
	LoadGame_MenuInit();
	UI_PushMenu( LoadGame_MenuDraw, LoadGame_MenuKey, NULL );
}


/*
=============================================================================

SAVEGAME MENU

=============================================================================
*/

void SaveGameCallback( void *self )
{
	menuaction_s *a = ( menuaction_s * ) self;

	Cbuf_AddText (va("save vrsave%02i\n", a->generic.localdata[0] ));
	UI_ForceMenuOff ();
}

void SaveGame_MenuDraw( void )
{
	Menu_DrawBanner( "m_banner_save_game" );
	Menu_AdjustCursor( &s_savegame_menu, 1 );
	Menu_Draw( &s_savegame_menu );
	DrawSaveshot (false);
}

void SaveGame_MenuInit( void )
{
	int32_t i;

	UI_UpdateSavegameData ();

	s_savegame_menu.x = SCREEN_WIDTH*0.5 - 240;
	s_savegame_menu.y = SCREEN_HEIGHT*0.5 - 58;
	s_savegame_menu.nitems = 0;

//	Load_Savestrings ();

	// don't include the autosave slot
	for ( i = 0; i < MAX_SAVEGAMES-1; i++ )
	{
		s_savegame_actions[i].generic.name = m_savestrings[i+1];
		s_savegame_actions[i].generic.localdata[0] = i+1;
		s_savegame_actions[i].generic.flags = QMF_LEFT_JUSTIFY;
		s_savegame_actions[i].generic.callback = SaveGameCallback;

		s_savegame_actions[i].generic.x = 0;
		s_savegame_actions[i].generic.y = ( i ) * MENU_LINE_SIZE;

		s_savegame_actions[i].generic.type = MTYPE_ACTION;

		Menu_AddItem( &s_savegame_menu, &s_savegame_actions[i] );
	}
	s_savegame_back_action.generic.type	= MTYPE_ACTION;
	s_savegame_back_action.generic.flags  = QMF_LEFT_JUSTIFY;
	s_savegame_back_action.generic.x		= 0;
	s_savegame_back_action.generic.y		= (MAX_SAVEGAMES+1)*MENU_LINE_SIZE;
	s_savegame_back_action.generic.name		= " back";
	s_savegame_back_action.generic.callback = UI_BackMenu;

	Menu_AddItem( &s_savegame_menu, &s_savegame_back_action );
}

const char *SaveGame_MenuKey( int32_t key )
{
	if ( key == K_ENTER || key == K_ESCAPE )
	{
		s_loadgame_menu.cursor = s_savegame_menu.cursor - 1;
		if ( s_loadgame_menu.cursor < 0 )
			s_loadgame_menu.cursor = 0;
	}
	return Default_MenuKey( &s_savegame_menu, key );
}

void M_Menu_SaveGame_f (void)
{
	if (!Com_ServerState())
		return;		// not playing a game

	SaveGame_MenuInit();
	UI_PushMenu( SaveGame_MenuDraw, SaveGame_MenuKey, NULL );
//	Load_Savestrings ();
}
