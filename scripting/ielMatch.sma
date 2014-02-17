// Uncomment for SQL version
// #define USING_SQL

#include <amxmodx>
#include <amxmisc>
#include <cstrike>
#include <fakemeta>
#include <hamsandwich>
#include <engine>
#include <fun>
#include <sockets>

#if defined USING_SQL
#include <sqlx>
#endif

#define PLUGIN	"iel Match Plugins"
#define VERSION	"0.1"
#define AUTHOR	"iel"
#define LOADING	"^n^t%s v%s, Copyright (C) 2014 by %s^n"

#define IM_ACCESS ADMIN_LEVEL_A
#define MAX_CLIENTS 32

#include "include/ielMatch/core.inl"
#include "include/ielMatch/teamtalk.inl"
#include "include/ielMatch/team_money.inl"
#include "include/ielMatch/anti_silentrun.inl"
#include "include/ielMatch/ready.inl"
#include "include/ielMatch/match.inl"

public plugin_init()
{
	register_plugin(PLUGIN, VERSION, AUTHOR);
	server_print(LOADING,PLUGIN,VERSION,AUTHOR);
	register_dictionary("im_teamtalk.txt");
	register_dictionary("im_team_money.txt");
	register_dictionary("im_anti_silentrun.txt");
	register_dictionary("im_ready.txt");
	register_dictionary("im_match.txt");

	register_event("TextMsg", "restart_round", "a", "2&#Game_C", "2&#Game_w");
	register_event("SendAudio", "end_round", "a", "2&%!MRAD_terwin", "2&%!MRAD_ctwin", "2&%!MRAD_rounddraw");

	RegisterHam(Ham_Spawn, "player", "FwdPlayerSpawn", 1);

	tt_plugin_init();
	tm_plugin_init();
	asr_plugin_init();
	rdy_plugin_init();

	rdy_set_task();
}

public plugin_cfg()
{
	tm_plugin_cfg();
}

public client_connect(id)
{
	tt_client_connect(id);
}

public client_disconnect(id)
{
	rdy_client_disconnect(id);
}

public client_authorized(id)
{
	server_msg(id);
}

public FwdPlayerSpawn(id)
{
	tm_FwdPlayerSpawn(id);
	return HAM_IGNORED;
}

public restart_round()
{
	tm_restart_round();
}

public end_round()
{
	tm_end_round();
}

public server_msg(id)
{
	client_cmd(id,"echo ");
	client_cmd(id,"echo > - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - <");
	client_cmd(id,"echo > - Server is running iel Match plugin by iel");
	client_cmd(id,"echo > - (C) All Copyright Reserved !");
	client_cmd(id,"echo > - Mailto: wxc@wxccs.org");
	client_cmd(id,"echo > - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - <");
	client_cmd(id,"echo ");
	engclient_cmd(id,"setinfo","Auto Match plugin %s",VERSION);    
}