enum cc_Color
{
	NORMAL = 1, // clients scr_concolor cvar color
	GREEN, // Green Color
	TEAM_COLOR, // Red, grey, blue
	GREY, // grey
	RED, // Red
	BLUE, // Blue
}

enum EXPORTCMD
{
	MATCHNAME,
}

new Export[EXPORTCMD][125];

new cc_TeamName[][] = 
{
	"",
	"TERRORIST",
	"CT",
	"SPECTATOR"
}

new CsInternalModel:_ModelsRandom[8] = 
{
	CS_T_TERROR,
	CS_T_LEET,
	CS_T_ARCTIC,
	CS_T_GUERILLA,
	CS_CT_URBAN,
	CS_CT_GSG9,
	CS_CT_GIGN,
	CS_CT_SAS
}

new bool:core_g_ChangeOneModels[33];
new core_g_Hostname;

public core_plug_init()
{
	register_srvcmd("im_matchname", "core_get_matchname");

	register_clcmd("say thetime", "core_GetTime");
	register_clcmd("say time", "core_GetTime");
	register_clcmd("say sj", "core_GetTime");

	core_g_Hostname = get_cvar_pointer("hostname");
}

public core_get_matchname()
{
	new ThisCmd[64];
	read_argv(0, ThisCmd, 63);
	if(equal(ThisCmd, "im_matchname"))
		read_argv(1, Export[MATCHNAME], 63);
}

stock ShowDirectorMessage(id, Float:x, Float:y, r, g, b, effect, Float:fadeintime, Float:fadeouttime, Float:holdtime, Float:fxtime, msg[], {Float, Sql, Result, _}:...)
{
	new text[128];
	vformat(text, charsmax(text), msg, 13);
	new len = strlen(text);
	
	if(!len) return;
	if(id)
		message_begin(MSG_ONE_UNRELIABLE, SVC_DIRECTOR, _, id);
	else
		message_begin(MSG_BROADCAST, SVC_DIRECTOR, _, 0);
	
	write_byte(31+len);	// command length
	write_byte(DRC_CMD_MESSAGE);		// command_event
	write_byte(effect);		// effect
	write_byte(b);	// b
	write_byte(g);	// g
	write_byte(r);	// r
	write_byte(0);	// a
	write_long(_:x);		// x
	write_long(_:y);		// y
	write_long(_:fadeintime);		// fade in time
	write_long(_:fadeouttime);		// fade out time
	write_long(_:holdtime);	// hold time
	write_long(_:fxtime);	// [optional] effect time - time the highlight lags behing the leading text in effect 2
	write_string(text);	// string text message
	message_end();
}

ColorChat(id, cc_Color:type, const msg[], {Float,Sql,Result,_}:...)
{
	new message[256];

	switch(type)
	{
		case NORMAL: // clients scr_concolor cvar color
		{
			message[0] = 0x01;
		}
		case GREEN: // Green
		{
			message[0] = 0x04;
		}
		default: // White, Red, Blue
		{
			message[0] = 0x03;
		}
	}

	vformat(message[1], 251, msg, 4);

	// Make sure message is not longer than 192 character. Will crash the server.
	message[192] = '^0';

	new team, ColorChange, index, MSG_Type;
	
	if(id)
	{
		MSG_Type = MSG_ONE;
		index = id;
	} else {
		index = cc_FindPlayer();
		MSG_Type = MSG_ALL;
	}
	
	team = get_user_team(index);
	ColorChange = cc_ColorSelection(index, MSG_Type, type);

	cc_ShowColorMessage(index, MSG_Type, message);
		
	if(ColorChange)
	{
		cc_Team_Info(index, MSG_Type, cc_TeamName[team]);
	}
}

cc_FindPlayer()
{
	new i = -1;

	while(i <= get_maxplayers())
	{
		if(is_user_connected(++i))
			return i;
	}

	return -1;
}

cc_ColorSelection(index, type, cc_Color:Type)
{
	switch(Type)
	{
		case RED:
		{
			return cc_Team_Info(index, type, cc_TeamName[1]);
		}
		case BLUE:
		{
			return cc_Team_Info(index, type, cc_TeamName[2]);
		}
		case GREY:
		{
			return cc_Team_Info(index, type, cc_TeamName[0]);
		}
	}

	return 0;
}

cc_ShowColorMessage(id, type, message[])
{
	static bool:saytext_used;
	static get_user_msgid_saytext;
	if(!saytext_used)
	{
		get_user_msgid_saytext = get_user_msgid("SayText");
		saytext_used = true;
	}
	message_begin(type, get_user_msgid_saytext, _, id);
	write_byte(id)		
	write_string(message);
	message_end();	
}

cc_Team_Info(id, type, team[])
{
	static bool:teaminfo_used;
	static get_user_msgid_teaminfo;
	if(!teaminfo_used)
	{
		get_user_msgid_teaminfo = get_user_msgid("TeamInfo");
		teaminfo_used = true;
	}
	message_begin(type, get_user_msgid_teaminfo, _, id);
	write_byte(id);
	write_string(team);
	message_end();

	return 1;
}

public is_player(id)
{
	if (is_user_connected(id) && !is_user_bot(id) && !is_user_hltv(id))
		return true;
	return false;
}

public core_swap_teams()
{
	new playersCT[32];
	new playersT[32];
	new nbrCT, nbrT;
	
	client_print(0,print_chat,"* [iM] %L", LANG_PLAYER, "SWITCHING_TEAMS");
	
	get_players(playersCT,nbrCT,"e","CT");
	get_players(playersT,nbrT,"e","TERRORIST");

	for(new i = 0; i < nbrCT; i++)
	{
		cs_set_user_team(playersCT[i], CS_TEAM_T);

		client_print(playersCT[i],print_chat,"* [iM] %L", playersCT[i], "NOW_ON_T");
	}

	for(new i = 0; i < nbrT; i++)
	{
		cs_set_user_team(playersT[i], CS_TEAM_CT);
		
		client_print(playersT[i], print_chat, "* [iM] %L", playersT[i], "NOW_ON_CT");
	}

	return PLUGIN_CONTINUE;
}

public core_OpenModelMenu(id)
{
	new menu = menu_create("\ySelect your appearance", "core_ModelMenuHandler");
	if(cs_get_user_team(id) == CS_TEAM_T)
	{
		menu_additem(menu, "Phoenix Connexion", "1", 0);
		menu_additem(menu, "Elite Crew", "2", 0);
		menu_additem(menu, "Arctic Avengers", "3", 0);
		menu_additem(menu, "Guerilla Warfare", "4", 0);
	}
	else if(cs_get_user_team(id) == CS_TEAM_CT)
	{
		menu_additem(menu, "Seal Team 6", "1", 0);
		menu_additem(menu, "GSG-9", "2", 0);
		menu_additem(menu, "SAS", "3", 0);
		menu_additem(menu, "GIGN", "4", 0);
	}
	menu_setprop(menu, MPROP_EXITNAME, "EXIT");
	menu_display(id, menu, 0);
}

public core_ModelMenuHandler(id, menu, item)
{
	if(item == MENU_EXIT)
	{
		menu_destroy(menu);
		return PLUGIN_HANDLED;
	}
	new data[6], iName[64];
	new access, callback;
	menu_item_getinfo(menu, item, access, data,5, iName, 63, callback);
	new key = str_to_num(data);
	
	if(!is_player(id)) return PLUGIN_HANDLED;

	core_g_ChangeOneModels[id] = true;
	
	if(cs_get_user_team(id) == CS_TEAM_T)
	{
		switch(key)
		{
			case 1: cs_set_user_team(id, CS_TEAM_T, _ModelsRandom[0]);
			case 2: cs_set_user_team(id, CS_TEAM_T, _ModelsRandom[1]);
			case 3: cs_set_user_team(id, CS_TEAM_T, _ModelsRandom[2]);
			case 4: cs_set_user_team(id, CS_TEAM_T, _ModelsRandom[3]);
		}
	}
	else if(cs_get_user_team(id) == CS_TEAM_CT)
	{
		switch(key)
		{
			case 1: cs_set_user_team(id, CS_TEAM_CT, _ModelsRandom[4]);
			case 2: cs_set_user_team(id, CS_TEAM_CT, _ModelsRandom[5]);
			case 3: cs_set_user_team(id, CS_TEAM_CT, _ModelsRandom[7]);
			case 4: cs_set_user_team(id, CS_TEAM_CT, _ModelsRandom[6]);
		}
	}
	menu_destroy(menu);
	return PLUGIN_HANDLED;
}

public core_client_putinserver(id)
{
	core_g_ChangeOneModels[id] = false;
}

public core_FwdPlayerSpawn(id)
{
	core_g_ChangeOneModels[id] = false;
}

public core_GetTime(id)
{
	new MONTHS[12][] = {"1","2","3","4","5","6","7","8","9" ,"10","11","12"}
	new WEEK[7][] = {"一","二","三","四","五","六","日"}
	new DAY[31][] = {"1","2","3","4","5","6","7","8","9","10","11","12","13","14","15","16","17","18","19","20","21","22","23","24","25","26","27","28","29","30","31"}
	new ns_Hour[3], ns_Minutes[3], ns_Month[3], ns_Day[3], ns_Year[5], ns_second[3], ns_week[5];
	
	get_time("%H", ns_Hour, charsmax(ns_Hour));
	get_time("%M", ns_Minutes, charsmax(ns_Minutes));
	get_time("%m", ns_Month, charsmax(ns_Month));
	get_time("%d", ns_Day, charsmax(ns_Day));
	get_time("%Y", ns_Year, charsmax(ns_Year));
	get_time("%S", ns_second, charsmax(ns_second));
	get_time("%w", ns_week, charsmax(ns_week));
	
	if(!str_to_num(ns_week))
		ColorChat(id, TEAM_COLOR, "【%s年%s月%s日 星期%s】", ns_Year, MONTHS[str_to_num(ns_Month) -1], DAY[str_to_num(ns_Day) -1], WEEK[str_to_num(ns_week) + 6]);
	else
		ColorChat(id, TEAM_COLOR, "【%s年%s月%s日 星期%s】", ns_Year, MONTHS[str_to_num(ns_Month) -1], DAY[str_to_num(ns_Day) -1], WEEK[str_to_num(ns_week) - 1]);
	
	ColorChat(id, TEAM_COLOR, "【北京时间：%s:%s:%s】", ns_Hour, ns_Minutes, ns_second);
	
	return PLUGIN_HANDLED;
}

stock SetServerName(msg[], {Float,Sql,Result,_}:...)
{
	static text[64];
	vformat(text, charsmax(text), msg, 2);
	message_begin(MSG_ALL, get_user_msgid("ServerName"), {0,0,0}, 0);
	write_string(text);
	message_end();
	
	console_print(0, text);
}

public fnCompare(elem1[], elem2[])
{
	if(elem1[1] > elem2[1]) 
		return -1;
	else if(elem1[1] < elem2[1])
		return 1;
	
	return 0;
}
