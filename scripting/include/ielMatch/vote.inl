#define VOTE_TASK_ID_MAP 1000000
new bool:vote_gb_map_voting = false;
new vote_gn_map_voting_limits = 0;
new vote_ga_map_voting_result[7];
new bool:vote_gb_team_voting;
new vote_gm_team_voting;
new vote_ga_team_voting_result[2];
new bool:vote_gb_player_voting = false;

new const vote_ga_MapName[][] =
{
	"de_dust2", 
	"de_inferno",
	"de_train",
	"de_nuke", 	
	"de_tuscan",
	"de_mirage",
	"de_forge"
}

public vote_get_is_vote_map() return vote_gb_map_voting;
public vote_set_is_vote_map(bool:val) vote_gb_map_voting = val;
public vote_get_is_team_voting() return vote_gb_team_voting;
public vote_set_is_team_voting(bool:val) vote_gb_team_voting = val;
public vote_get_is_player_voting() return vote_gb_player_voting;
public vote_set_is_player_voting(bool:val) vote_gb_player_voting = val;

public vote_plugin_init()
{
	register_clcmd("say votemap", "vote_check_access", IM_ACCESS);
	register_clcmd("say vote", "vote_player_cmd");
	vote_gb_team_voting = false;
	vote_ga_team_voting_result[0] = 0;
	vote_ga_team_voting_result[1] = 0;
}

public vote_check_access(id, level)
{
	if (!access(id, level) && match_get_inmatch())
	{
		console_print(id,"%L", id, "VOTE_NO_ACCESS");
		client_print(id, print_center, "%L", id, "VOTE_NO_ACCESS");
		return PLUGIN_HANDLED;
	}
	vote_map_menu();
	return PLUGIN_HANDLED;
}

public vote_knife_round_win(CsTeams:win_team)
{
	if (vote_get_is_team_voting()) return PLUGIN_HANDLED;

	new temp_voting_title[64], temp_voting_switch[16], temp_voting_keep[16], temp_voting_exit[16];

	formatex( temp_voting_title, charsmax(temp_voting_title), "%L", LANG_PLAYER, "VOTE_TEAM_TITLE" )
	formatex( temp_voting_switch, charsmax(temp_voting_switch), "%L", LANG_PLAYER, "SWITCH" )
	formatex( temp_voting_keep, charsmax(temp_voting_keep), "%L", LANG_PLAYER, "KEEP" )
	formatex( temp_voting_exit, charsmax(temp_voting_exit), "%L", LANG_PLAYER, "EXIT" )

	vote_gm_team_voting = menu_create(temp_voting_title, "vote_team_menu_handler");
	menu_additem(vote_gm_team_voting, temp_voting_switch, "0", 0);
	menu_additem(vote_gm_team_voting, temp_voting_keep, "1", 0);

	for(new tempid = 1; tempid <= get_maxplayers(); tempid++)
	{
		if(!is_player(tempid)) continue;
		if(cs_get_user_team(tempid) == win_team)
		{
			menu_setprop(vote_gm_team_voting, MPROP_EXITNAME, temp_voting_exit);
			menu_display(tempid, vote_gm_team_voting, 0);
			vote_set_is_team_voting(true);
		}
	}

	set_task(10.0, "vote_team_vote_end");
	return PLUGIN_HANDLED;
}

public vote_team_menu_handler(id, menu, item)
{
	if(!vote_get_is_team_voting()) return PLUGIN_HANDLED;
	
	if(item == MENU_EXIT)
		return PLUGIN_HANDLED;
	
	new data[6], iName[64];
	new access, callback;
	menu_item_getinfo(menu, item, access, data,5, iName, 63, callback);
	new voteid = str_to_num(data);
	vote_ga_team_voting_result[voteid]++;
	return PLUGIN_HANDLED;
}

public vote_team_vote_end()
{
	client_print(0, print_chat, "[iM] 交换：%d 保持：%d", vote_ga_team_voting_result[0], vote_ga_team_voting_result[1]);

	if(vote_ga_team_voting_result[0] > vote_ga_team_voting_result[1])
	{
		core_swap_teams();
		match_show_teams();
		match_start(4);
		
	}
	else
	{
		match_show_teams();
		match_start(4);
		
	}	
	vote_ga_team_voting_result[0] = 0;
	vote_ga_team_voting_result[1] = 0;
	vote_set_is_team_voting(false);
	menu_destroy(vote_gm_team_voting);
}

public vote_map_menu()
{
	if (vote_gb_map_voting)
	{
		ColorChat(0, NORMAL, "%L", LANG_PLAYER, "IN_VOTING");
		return;
	}
	if (vote_gn_map_voting_limits > 2)
	{
		ColorChat(0, NORMAL, "已经超过投票次数,无法执行.");
		return;
	}
	vote_gn_map_voting_limits++;
	remove_task(VOTE_TASK_ID_MAP);
	vote_gb_map_voting = true;
	new temp[10];
	new temp_voting_title[16], temp_voting_exit[16];
	formatex( temp_voting_title, charsmax(temp_voting_title), "\y%L:", LANG_PLAYER, "VOTE_MAP_TITLE" )
	formatex( temp_voting_exit, charsmax(temp_voting_exit), "%L", LANG_PLAYER, "EXIT" )
	
	new a, menu;
	for (new i = 1; i <= 32; i++){
		if (is_user_connected(i) && CS_TEAM_T <= cs_get_user_team(i) <= CS_TEAM_CT){
			menu = menu_create(temp_voting_title, "vote_map_menu_handler");
			for (a = 0; a < sizeof(vote_ga_MapName); a++)
			{
				num_to_str(a, temp, 9);
				menu_additem(menu, vote_ga_MapName[a], temp);
			}
			
			menu_setprop(menu, MPROP_EXITNAME, temp_voting_exit);
			menu_display(i, menu);
		}
	}
	set_task(30.0, "vote_map_end", VOTE_TASK_ID_MAP);
}

public vote_map_menu_handler(id, menu, item)
{
	new szName[32];
	get_user_name(id, szName, 31);
	
	if (item == MENU_EXIT)
	{
		ColorChat(id, TEAM_COLOR, "%s ^x01%L", szName, id, "VOTE_MAP_DROP");
		menu_destroy(menu);
		return PLUGIN_HANDLED;
	}
	
	new data[6], name[32];
	new key, access, callback;
	menu_item_getinfo(menu, item, access, data, 5, name, 31, callback);
	key = str_to_num(data);
	vote_ga_map_voting_result[key]++
	ColorChat(id, TEAM_COLOR, "%s ^x01%L ^x04%s", szName, id, "VOTE_MAP_CHOOSE", vote_ga_MapName[key]);
	menu_destroy(menu);
	return PLUGIN_HANDLED;
}

public vote_map_end(taskid)
{
	new temp[7][2];
	for (new i = 0; i < sizeof(vote_ga_MapName); i++)
	{
		temp[i][0] = i;
		temp[i][1] = vote_ga_map_voting_result[i];
	}
	SortCustom2D(temp, 7, "fnCompare");
	new param[1];
	param[0] = temp[0][0];
	new currentmap[32];
	get_mapname(currentmap, 31);
	if (equal(currentmap, vote_ga_MapName[param[0]]))
	{
		vote_gb_map_voting = false
		remove_task(VOTE_TASK_ID_MAP);
		if(match_get_inmatch())
		{
			ColorChat(0, NORMAL,"%L", LANG_PLAYER, "VOTE_MAP_NOCHANGE");
			return ;
		}
		else
		{
			ColorChat(0, NORMAL,"%L", LANG_PLAYER, "VOTE_MAP_REFRESH");
			set_task(31.0, "vote_map_changelevel", 0, param, 1)
			return ;
		}
	}
	ColorChat(0, NORMAL,"%L ^x04%s", LANG_PLAYER, "VOTE_MAP_CHANGETO",vote_ga_MapName[param[0]]);
	vote_gb_map_voting = false
	set_task(31.0, "vote_map_changelevel", 0, param, 1)
}

public vote_map_changelevel(param[])
{
	server_cmd("changelevel %s", vote_ga_MapName[param[0]]);
}

public vote_player_cmd(id)
{
	if(!vote_get_is_player_voting())
	{
		switch(cs_get_user_team(id))
		{
			case CS_TEAM_T:
			{

			}

			case CS_TEAM_CT:
			{

			}

			case CS_TEAM_SPECTATOR:
			{

			}

			default:
			{
				client_print(id, print_center, "%L", LANG_PLAYER, "VOTE_PLAYER_NOPERMIT");
			}
		}
	}
	else
	{
		client_print(id, print_center, "%L", LANG_PLAYER, "VOTE_PLAYER_INVOTING");
	}
}