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
#define VERSION	"1.0b1"
#define AUTHOR	"iel"
#define LOADING	"^n^t%s v%s, Copyright (C) 2014 by %s^n"

#define IM_ACCESS ADMIN_LEVEL_A
#define MAX_CLIENTS 32

#include "include/ielMatch/teamtalk.inl"
#include "include/ielMatch/team_money.inl"
#include "include/ielMatch/anti_silentrun.inl"
#include "include/ielMatch/core.inl"
#include "include/ielMatch/ready.inl"
#include "include/ielMatch/model.inl"
#include "include/ielMatch/match.inl"
#include "include/ielMatch/vote.inl"
#include "include/ielMatch/screenshot.inl"
#include "include/ielMatch/record.inl"


public plugin_init()
{
	register_plugin(PLUGIN, VERSION, AUTHOR);
	server_print(LOADING,PLUGIN,VERSION,AUTHOR);
	register_dictionary("im_teamtalk.txt");
	register_dictionary("im_team_money.txt");
	register_dictionary("im_anti_silentrun.txt");
	register_dictionary("im_ready.txt");
	register_dictionary("im_match.txt");
	register_dictionary("im_vote.txt");
	register_dictionary("im_record.txt");

	register_event("TextMsg", "restart_round", "a", "2&#Game_C", "2&#Game_w");
	register_event("SendAudio", "end_round", "a", "2&%!MRAD_terwin", "2&%!MRAD_ctwin", "2&%!MRAD_rounddraw");
	register_logevent("round_start", 2, "0=World triggered", "1=Round_Start");

	RegisterHam(Ham_Spawn, "player", "FwdPlayerSpawn", 1);
	

	core_plugin_init();
	model_plugin_init();
	tt_plugin_init();
	tm_plugin_init();
	asr_plugin_init();
	rdy_plugin_init();
	rec_plugin_init();
	match_plugin_init();
	vote_plugin_init();
	ss_plugin_init();

	rdy_set_task();
	match_start(0);
}

public plugin_cfg()
{
	new customdir[64];
	get_customdir(customdir, 63);
	server_cmd("exec ^"%s/ielMatch/setting.cfg^"", customdir);

	tm_plugin_cfg();
}

public client_connect(id)
{
	rdy_client_connect(id);
	model_client_connect(id);
	tt_client_connect(id);
	match_client_connect(id);
}

public client_disconnect(id)
{
	rdy_client_disconnect(id);
	match_client_disconnect(id);
	model_client_disconnect(id);
}

public client_authorized(id)
{
	server_msg(id);
}

public client_putinserver(id)
{
	core_client_putinserver(id);
	rec_client_putinserver(id);
}

public FwdPlayerSpawn(id)
{
	core_FwdPlayerSpawn(id);
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
	match_end_round();
}

public round_start()
{
	match_round_start();
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
	engclient_cmd(id,"setinfo","iel Match plugin %s",VERSION);    
}
