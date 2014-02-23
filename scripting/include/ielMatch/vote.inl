new bool:vote_gb_map_voting = false;
new vote_gm_team_voting;
new vote_ga_team_voting_result[2];
new vote_gn_TeamVoting;

public vote_get_is_vote_map() return vote_gb_map_voting;
public vote_set_is_vote_map(bool:val) vote_gb_map_voting = val;

public vote_knife_round_win(CsTeams:win_team)
{
	if (vote_gn_TeamVoting) return PLUGIN_HANDLED;

	new temp_voting_title[64], temp_voting_ct[16], temp_voting_t[16], temp_voting_exit[16];

	formatex( temp_voting_title, charsmax(temp_voting_title), "%L", LANG_PLAYER, "VOTE_TEAM_TITLE" )
	formatex( temp_voting_ct, charsmax(temp_voting_ct), "%L", LANG_PLAYER, "CT" )
	formatex( temp_voting_t, charsmax(temp_voting_t), "%L", LANG_PLAYER, "T" )
	formatex( temp_voting_exit, charsmax(temp_voting_exit), "%L", LANG_PLAYER, "EXIT" )

	vote_gm_team_voting = menu_create(temp_voting_title, "vote_team_menu_handler");
	menu_additem(vote_gm_team_voting, temp_voting_t, "0", 0);
	menu_additem(vote_gm_team_voting, temp_voting_ct, "1", 0);

	for(new tempid = 1; tempid <= get_maxplayers(); tempid++)
	{
		if(!is_player(tempid)) continue;
		if(cs_get_user_team(tempid) == win_team)
		{
			menu_setprop(vote_gm_team_voting, MPROP_EXITNAME, temp_voting_exit);
			menu_display(tempid, vote_gm_team_voting, 0);
			vote_gn_TeamVoting++;
		}
	}

	if(win_team == CS_TEAM_T) set_task(10.0, "vote_team_vote_end", _,"CS_TEAM_T");
	if(win_team == CS_TEAM_CT) set_task(10.0, "vote_team_vote_end", _,"CS_TEAM_CT");
	return PLUGIN_HANDLED;
}

public vote_team_menu_handler(id, menu, item)
{
	if(!vote_gn_TeamVoting) return PLUGIN_HANDLED;
	
	if(item == MENU_EXIT)
		return PLUGIN_HANDLED;
	
	new data[6], iName[64];
	new access, callback;
	menu_item_getinfo(menu, item, access, data,5, iName, 63, callback);
	new voteid = str_to_num(data);
	vote_ga_team_voting_result[voteid]++;
	return PLUGIN_HANDLED;
}

public vote_team_vote_end(args[])
{
	new CsTeams:win_team;
	if(equal(args, "CS_TEAM_T")) win_team = CS_TEAM_T;
	else if(equal(args, "CS_TEAM_CT")) win_team = CS_TEAM_CT;

	if(vote_ga_team_voting_result[0] > vote_ga_team_voting_result[1])
	{
		if(win_team == CS_TEAM_CT) core_swap_team();
		match_show_teams();
		match_start(4);
		
	}
	else if(vote_ga_team_voting_result[1] > vote_ga_team_voting_result[0])
	{
		if(win_team == CS_TEAM_T) core_swap_team();
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
	vote_gn_TeamVoting = 0;
	menu_destroy(vote_gm_team_voting);
}
