new bool:match_knife_round;
new bool:match_inmatch;
new bool:match_issechalf;
new bool:match_isovertime;
new bool:match_iswarm;
new match_team_a_score[2], match_team_b_score[2], match_total_score[2];
new match_p_allow_show_score;
new match_p_allow_ot;
new match_p_allow_changetag, match_p_teamatag, match_p_teambtag, match_ga_player_orig_name[33][128];
new MATCH_MSG_TR, MATCH_MSG_CT;


public match_get_inmatch() return match_inmatch;
public match_get_iskniferound() return match_knife_round;
public match_get_issechalf() return match_issechalf;
public match_get_isot() return match_isovertime;
public match_get_iswarm() return match_iswarm;
public match_set_inmatch(bool:val) { match_inmatch = val; }
public match_set_iskniferound(bool:val) { match_knife_round = val; }
public match_set_issechalf(bool:val) { match_issechalf = val; }
public match_set_isot(bool:val) { match_isovertime = val; }
public match_set_iswarm(bool:val) { match_iswarm = val; }

public match_plugin_init()
{
	match_p_allow_show_score = register_cvar("im_showscore","1");
	match_p_allow_ot = register_cvar("im_overtime","0");
	match_p_allow_changetag = register_cvar("im_teamtag","1");
	match_p_teamatag = register_cvar("im_teamatag","TeamA");
	match_p_teambtag = register_cvar("im_teambtag","TeamB");

	register_concmd("knife", "match_force_start_knife", IM_ACCESS);
	register_concmd("r3", "match_force_start_common", IM_ACCESS);

	register_event("CurWeapon", "match_weapon_detect", "be", "1=1");

	register_message(get_user_msgid("TextMsg"), "match_knife_check_timesup");

	MATCH_MSG_TR = CreateHudSyncObj();
	MATCH_MSG_CT = CreateHudSyncObj();

	match_team_a_score[0] = 0;
	match_team_a_score[1] = 0;

	match_team_b_score[0] = 0;
	match_team_b_score[1] = 0;

	match_total_score[0] = 0;
	match_total_score[1] = 0;
}

public match_client_connect(id)
{
	get_user_name(id, match_ga_player_orig_name[id], 128);
}

public match_client_disconnect(id)
{
	match_ga_player_orig_name[id] = "";
}

public match_force_start_knife (id, level)
{
	if (match_get_inmatch() || match_get_iskniferound()) return PLUGIN_HANDLED;
	if (!access(id, level))
	{
		console_print(id,"%L", id, "NO_ACCESS");
		client_print(id, print_center, "%L", id, "NO_ACCESS");
		return PLUGIN_HANDLED;
	}
	match_start(1);
	return PLUGIN_HANDLED;
}

public match_force_start_common (id, level)
{
	if (match_get_inmatch() || match_get_iskniferound()) return PLUGIN_HANDLED;
	if (!access(id, level))
	{
		console_print(id,"%L", id, "NO_ACCESS");
		client_print(id, print_center, "%L", id, "NO_ACCESS");
		return PLUGIN_HANDLED;
	}
	match_start(4);
	return PLUGIN_HANDLED;
}

public match_restart(time[])
{
	set_cvar_string("sv_restart", time);
	
	return PLUGIN_CONTINUE;
}

public match_load_config(matchtype)
{
	new customdir[64];
	get_customdir(customdir, 63);
	switch(matchtype)
	{
		case 1:
		{
			server_cmd("exec ^"%s/ielMatch/match_knife_config.cfg^"", customdir);
			return PLUGIN_CONTINUE;
		}
		case 2:
		{
			server_cmd("exec ^"%s/ielMatch/match_ot_config.cfg^"", customdir);
			return PLUGIN_CONTINUE;
		}
		case 3:
		{
			server_cmd("exec ^"%s/ielMatch/match_ot_config.cfg^"", customdir);
			return PLUGIN_CONTINUE;
		}
		case 4:
		{
			server_cmd("exec ^"%s/ielMatch/match_config.cfg^"", customdir);
			return PLUGIN_CONTINUE;
		}
		case 5:
		{
			server_cmd("exec ^"%s/ielMatch/match_config.cfg^"", customdir);
			return PLUGIN_CONTINUE;
		}
		default:
		{
			server_cmd("exec ^"%s/ielMatch/warmup_config.cfg^"", customdir);
			return PLUGIN_CONTINUE;
		}
	}

	return PLUGIN_CONTINUE;
}

public match_r3(matchtype)
{
	match_load_config(matchtype);
	switch(matchtype)
	{
		case 1:
		{
			ColorChat( 0, GREY, "^x04%s^x03%L",Export[MATCHNAME], LANG_PLAYER, "KNIFE_R3");
			set_task(2.0,"match_restart", _, "1", 1, "a", 3);
			ColorChat( 0, GREY, "^x04%s^x03%L",Export[MATCHNAME], LANG_PLAYER, "KNIFE_ROUND");
			ColorChat( 0, GREY, "^x04%s^x03%L",Export[MATCHNAME], LANG_PLAYER, "KNIFE_ROUND");
			ColorChat( 0, GREY, "^x04%s^x03%L",Export[MATCHNAME], LANG_PLAYER, "KNIFE_ROUND");
			ColorChat( 0, GREY, "^x04%s^x03%L",Export[MATCHNAME], LANG_PLAYER, "KNIFE_ROUND");
			ColorChat( 0, GREY, "^x04%s^x03%L",Export[MATCHNAME], LANG_PLAYER, "KNIFE_ROUND");
			ShowDirectorMessage(0, -1.0, 0.30, 0, 200, 0, 0, 0.8, 0.8, 6.0, 2.0, "--[ :: !! KNIFE ROUND !! :: ]--^n--[ :: !! KNIFE ROUND !! :: ]--^n--[ :: !! KNIFE ROUND !! :: ]--");
			return PLUGIN_CONTINUE;
		}
		case 2:
		{
			set_task(2.0,"match_restart", _, "1", 1);
			set_task(6.0,"match_restart", _, "1", 1);
			set_task(9.0,"match_restart", _, "1", 1);
			ColorChat( 0, GREY, "^x04%s^x03%L",Export[MATCHNAME], LANG_PLAYER, "MATCH_OTGL");
			ColorChat( 0, GREY, "^x04%s^x03%L",Export[MATCHNAME], LANG_PLAYER, "MATCH_OTGL");
			ColorChat( 0, GREY, "^x04%s^x03%L",Export[MATCHNAME], LANG_PLAYER, "MATCH_OTGL");
			ColorChat( 0, GREY, "^x04%s^x03%L",Export[MATCHNAME], LANG_PLAYER, "MATCH_OTGL");
			ColorChat( 0, GREY, "^x04%s^x03%L",Export[MATCHNAME], LANG_PLAYER, "MATCH_OTGL");
			ShowDirectorMessage(0, -1.0, 0.30, 0, 200, 0, 0, 0.8, 0.8, 6.0, 2.0, "--[  Over Time LIVE !!! GL & HF !!!  ]--^n--[  Over Time LIVE !!! GL & HF !!!  ]--^n--[  Over Time LIVE !!! GL & HF !!!  ]--");
			return PLUGIN_CONTINUE;
		}
		case 3:
		{
			set_task(2.0,"match_restart", _, "1", 1);
			set_task(6.0,"match_restart", _, "1", 1);
			set_task(9.0,"match_restart", _, "1", 1);
			ColorChat( 0, GREY, "^x04%s^x03%L",Export[MATCHNAME], LANG_PLAYER, "MATCH_OTGL");
			ColorChat( 0, GREY, "^x04%s^x03%L",Export[MATCHNAME], LANG_PLAYER, "MATCH_OTGL");
			ColorChat( 0, GREY, "^x04%s^x03%L",Export[MATCHNAME], LANG_PLAYER, "MATCH_OTGL");
			ColorChat( 0, GREY, "^x04%s^x03%L",Export[MATCHNAME], LANG_PLAYER, "MATCH_OTGL");
			ColorChat( 0, GREY, "^x04%s^x03%L",Export[MATCHNAME], LANG_PLAYER, "MATCH_OTGL");
			ShowDirectorMessage(0, -1.0, 0.30, 0, 200, 0, 0, 0.8, 0.8, 6.0, 2.0, "--[  Over Time LIVE !!! GL & HF !!!  ]--^n--[  Over Time LIVE !!! GL & HF !!!  ]--^n--[  Over Time LIVE !!! GL & HF !!!  ]--");
			return PLUGIN_CONTINUE;
		}
		case 4:
		{
			set_task(2.0,"match_restart", _, "1", 1);
			set_task(6.0,"match_restart", _, "1", 1);
			set_task(9.0,"match_restart", _, "1", 1);
			ColorChat( 0, GREY, "^x04%s^x03%L",Export[MATCHNAME], LANG_PLAYER, "MATCH_GL");
			ColorChat( 0, GREY, "^x04%s^x03%L",Export[MATCHNAME], LANG_PLAYER, "MATCH_GL");
			ColorChat( 0, GREY, "^x04%s^x03%L",Export[MATCHNAME], LANG_PLAYER, "MATCH_GL");
			ColorChat( 0, GREY, "^x04%s^x03%L",Export[MATCHNAME], LANG_PLAYER, "MATCH_GL");
			ColorChat( 0, GREY, "^x04%s^x03%L",Export[MATCHNAME], LANG_PLAYER, "MATCH_GL");
			ShowDirectorMessage(0, -1.0, 0.30, 0, 200, 0, 0, 0.8, 0.8, 6.0, 2.0, "--[  Game LIVE !!! GL & HF !!!  ]--^n--[  Game LIVE !!! GL & HF !!!  ]--^n--[  Game LIVE !!! GL & HF !!!  ]--");
			return PLUGIN_CONTINUE;
		}
		case 5:
		{
			set_task(2.0,"match_restart", _, "1", 1);
			set_task(6.0,"match_restart", _, "1", 1);
			set_task(9.0,"match_restart", _, "1", 1);
			ColorChat( 0, GREY, "^x04%s^x03%L",Export[MATCHNAME], LANG_PLAYER, "MATCH_GL");
			ColorChat( 0, GREY, "^x04%s^x03%L",Export[MATCHNAME], LANG_PLAYER, "MATCH_GL");
			ColorChat( 0, GREY, "^x04%s^x03%L",Export[MATCHNAME], LANG_PLAYER, "MATCH_GL");
			ColorChat( 0, GREY, "^x04%s^x03%L",Export[MATCHNAME], LANG_PLAYER, "MATCH_GL");
			ColorChat( 0, GREY, "^x04%s^x03%L",Export[MATCHNAME], LANG_PLAYER, "MATCH_GL");
			ShowDirectorMessage(0, -1.0, 0.30, 0, 200, 0, 0, 0.8, 0.8, 6.0, 2.0, "--[  Game LIVE !!! GL & HF !!!  ]--^n--[  Game LIVE !!! GL & HF !!!  ]--^n--[  Game LIVE !!! GL & HF !!!  ]--");
			return PLUGIN_CONTINUE;
		}
		default:
		{
			ColorChat( 0, GREY, "^x04%s^x03%L",Export[MATCHNAME], LANG_PLAYER, "WARMUP");
			set_task(2.0,"match_restart", _, "1", 1);
			ShowDirectorMessage(0, -1.0, 0.30, 0, 200, 0, 0, 0.8, 0.8, 6.0, 2.0, "--[  Warmup !!!  ]--^n--[  Warmup !!!  ]--^n--[  Warmup !!!  ]--");
			return PLUGIN_CONTINUE;
		}
	}

	return PLUGIN_CONTINUE;
}

public match_start(matchtype)
{
	switch(matchtype)
	{
		case 1:
		{
			//Knife Round
			match_r3(matchtype);
			match_set_inmatch(true);
			match_set_iskniferound(true);
			match_set_issechalf(false);
			match_set_isot(false);
			match_set_iswarm(false);
			return PLUGIN_CONTINUE;
		}
		case 2:
		{
			//OverTime Round
			match_r3(matchtype);
			match_set_inmatch(true);
			match_set_iskniferound(false);
			match_set_issechalf(false);
			match_set_isot(true);
			match_set_iswarm(false);
			rec_pov_demo_start();
			return PLUGIN_CONTINUE;
		}
		case 3:
		{
			//OverTime Second Half
			
			match_r3(matchtype);
			match_set_inmatch(true);
			match_set_iskniferound(false);
			match_set_issechalf(true);
			match_set_isot(true);
			match_set_iswarm(false);
			rec_pov_demo_start();
			return PLUGIN_CONTINUE;
		}
		case 4:
		{
			//Common Round
			match_r3(matchtype);
			match_set_inmatch(true);
			match_set_iskniferound(false);
			match_set_issechalf(false);
			match_set_isot(false);
			match_set_iswarm(false);
			rec_pov_demo_start();
			set_task(1.0, "rec_hltv_demo_start");
			return PLUGIN_CONTINUE;
		}
		case 5:
		{
			//Common Second Half
			match_r3(matchtype);
			match_set_inmatch(true);
			match_set_iskniferound(false);
			match_set_issechalf(true);
			match_set_isot(false);
			match_set_iswarm(false);
			rec_pov_demo_start();
			return PLUGIN_CONTINUE;
		}
		default:
		{
			//Warmup Round
			match_set_inmatch(false);
			match_set_iskniferound(false);
			match_set_issechalf(false);
			match_set_isot(false);
			match_set_iswarm(true);
			match_r3(matchtype);
			return PLUGIN_CONTINUE;
		}
	}

	return PLUGIN_CONTINUE;
}

public match_stop(matchtype)
{
	switch(matchtype)
	{
		case 1:
		{
			match_set_inmatch(false);
			match_set_iskniferound(false);
			match_set_issechalf(false);
			match_set_isot(false);
			match_set_iswarm(false);

			return PLUGIN_CONTINUE;
		}
		case 2:
		{
			match_set_inmatch(false);
			match_set_iskniferound(false);
			match_set_issechalf(false);
			match_set_isot(false);
			match_set_iswarm(false);

			set_task(1.0,"rec_pov_demo_stop");
			
			if(ss_get_allow())ss_screenshot;

			return PLUGIN_CONTINUE;
		}
		case 3:
		{
			match_set_inmatch(false);
			match_set_iskniferound(false);
			match_set_issechalf(false);
			match_set_isot(false);
			match_set_iswarm(false);

			set_task(1.0,"rec_pov_demo_stop");

			if(ss_get_allow())ss_screenshot;

			return PLUGIN_CONTINUE;
		}
		case 4:
		{
			match_set_inmatch(false);
			match_set_iskniferound(false);
			match_set_issechalf(false);
			match_set_isot(false);
			match_set_iswarm(false);

			set_task(1.0,"rec_pov_demo_stop");

			if(ss_get_allow())ss_screenshot;

			return PLUGIN_CONTINUE;
		}
		case 5:
		{
			match_set_inmatch(false);
			match_set_iskniferound(false);
			match_set_issechalf(false);
			match_set_isot(false);
			match_set_iswarm(false);

			set_task(1.0,"rec_pov_demo_stop");

			if(ss_get_allow())ss_screenshot;

			return PLUGIN_CONTINUE;
		}
		default:
		{
			match_set_inmatch(false);
			match_set_iskniferound(false);
			match_set_issechalf(false);
			match_set_isot(false);
			match_set_iswarm(false);

			return PLUGIN_CONTINUE;
		}
	}
	return PLUGIN_CONTINUE;
}

public match_end_round()
{

	match_change_team_tag();

	new param[12];
	read_data(2,param,8);
	if((!match_get_iskniferound() && match_get_inmatch()) && !match_get_issechalf())
	{
		if (param[7]=='t') match_team_a_score[0]++;
		else if (param[7]=='c') match_team_b_score[0]++;
	}
	else if((!match_get_iskniferound() && match_get_inmatch()) && match_get_issechalf())
	{
		if (param[7]=='c') match_team_a_score[1]++;
		else if (param[7]=='t') match_team_b_score[1]++;
	}

	client_print(0, print_chat, "[iM] %d %d %d %d %d %d", match_total_score[0], match_total_score[1], match_team_a_score[0], match_team_a_score[1], match_team_b_score[0],match_team_b_score[1]);
	
	if(get_pcvar_num(match_p_allow_show_score))
	{
		match_show_score(match_total_score,match_team_a_score,match_team_b_score);	
	}

	if(match_get_inmatch() && !match_get_isot())
	{
		if(!match_get_issechalf())
		{
			if(match_team_a_score[0] + match_team_b_score[0] == 15)
			{
				match_total_score[0] += match_team_a_score[0];
				match_total_score[1] += match_team_b_score[0];
				match_stop(4);
				set_task(1.5, "core_swap_teams");
				match_start(5);
			}
		}
		else
		{
			if(match_team_a_score[1] + match_team_b_score[1] == 15)
			{
				match_total_score[0] += match_team_a_score[1];
				match_total_score[1] += match_team_b_score[1];
				
				match_team_a_score[0] = 0;
				match_team_a_score[1] = 0;
				match_team_b_score[0] = 0;
				match_team_b_score[1] = 0;

				if(get_pcvar_num(match_p_allow_ot))
				{
					set_task(1.5, "core_swap_teams");
					match_stop(5);
					match_start(2);
				}
				else
				{
					match_stop(5);
					new HostName[64];
					get_pcvar_string(core_g_Hostname, HostName, charsmax(HostName));
					SetServerName(HostName);
					ColorChat( 0, GREY, "Match Finished & Good Game.");
					match_change_team_tag_back();
					set_task(5.0,"rec_hltv_demo_stop");
					set_task(3.0, "vote_map_menu");
					match_start(0);
				}
			}
			else if((match_team_a_score[0] + match_team_a_score[1] == 16) || match_team_b_score[0] + match_team_b_score[1] == 16)
			{
				match_total_score[0] += match_team_a_score[1];
				match_total_score[1] += match_team_b_score[1];
				match_stop(5);
				if (match_total_score[0] == 16) ColorChat( 0, RED, "Match Finished & Good Game.");
				else ColorChat( 0, BLUE, "Match Finished & Good Game.");
				new HostName[64];
				get_pcvar_string(core_g_Hostname, HostName, charsmax(HostName));
				SetServerName(HostName);
				match_change_team_tag_back();
				set_task(5.0,"rec_hltv_demo_stop");
				set_task(3.0, "vote_map_menu");
				match_start(0);
			}
		}
	}
	else if(match_get_inmatch() && match_get_isot())
	{
		if(!match_get_issechalf())
		{
			if(match_team_a_score[0] + match_team_b_score[0] == 3)
			{
				match_total_score[0] += match_team_a_score[0];
				match_total_score[1] += match_team_b_score[0];
				set_task(1.5, "core_swap_teams");
				match_stop(2);
				match_start(3);
			}
		}
		else
		{
			if(match_team_a_score[1] + match_team_b_score[1] == 3)
			{
				match_total_score[0] += match_team_a_score[1];
				match_total_score[1] += match_team_b_score[1];
				
				match_team_a_score[0] = 0;
				match_team_a_score[1] = 0;
				match_team_b_score[0] = 0;
				match_team_b_score[1] = 0;

				set_task(1.5, "core_swap_teams");
				match_stop(3);
				match_start(2);
			}
			else if((match_team_a_score[0] + match_team_a_score[1] == 4) || match_team_b_score[0] + match_team_b_score[1] == 4)
			{
				match_total_score[0] += match_team_a_score[1];
				match_total_score[1] += match_team_b_score[1];
				match_stop(3);
				if (match_total_score[0] == 4) ColorChat( 0, RED, "Match Finished & Good Game.");
				else ColorChat( 0, BLUE, "Match Finished & Good Game.");
				new HostName[64];
				get_pcvar_string(core_g_Hostname, HostName, charsmax(HostName));
				SetServerName(HostName);
				match_change_team_tag_back();
				set_task(5.0,"rec_hltv_demo_stop");
				set_task(3.0, "vote_map_menu");
				match_start(0);
			}
		}
	}
}

public match_show_score(total[],teama[],teamb[])
{
	if(match_get_inmatch() && !match_get_iskniferound())
	{
		new ScoreMsg[128];
		new teamatag[16], teambtag[16];
		if (get_pcvar_num(match_p_allow_changetag))
		{
			get_pcvar_string(match_p_teamatag, teamatag, charsmax(teamatag));
			get_pcvar_string(match_p_teambtag, teambtag, charsmax(teambtag));
		}
		else
		{
			teamatag = "Red Team";
			teambtag = "Blue Team";
		}
	
		if(!match_get_issechalf())
		{
			if ((total[0] + teama[0]) > (total[1] + teamb[0]))
			{
				formatex(ScoreMsg, charsmax(ScoreMsg), "%s vs %s | %d:%d", teamatag, teambtag, total[0] + teama[0],total[1] + teamb[0]);
				ColorChat( 0, RED, "%s %L %d-%d", teamatag,total[0] + teama[0], LANG_PLAYER, "MATCH_IS_WINNING",total[1] + teamb[0]);
			}
			else if ((total[0] + teama[0]) < (total[1] + teamb[0]))
			{
				formatex(ScoreMsg, charsmax(ScoreMsg), "%s vs %s | %d:%d", teambtag, teamatag, total[1] + teamb[0],total[0] + teama[0]);
				ColorChat( 0, BLUE, "%s %L %d-%d", teambtag, total[1] + teamb[0], LANG_PLAYER, "MATCH_IS_WINNING",total[0] + teama[0]);
			}
			else
			{
				formatex(ScoreMsg, charsmax(ScoreMsg), "%s vs %s | %d:%d", teamatag, teambtag, total[1] + teamb[0],total[0] + teama[0]);	
				ColorChat( 0, GREY, "%L %d-%d",total[1] + teamb[0], LANG_PLAYER, "MATCH_IS_TIED",total[0] + teama[0]);
			}
			SetServerName(ScoreMsg);
		}
		else
		{
			if ((total[0] + teama[1]) > (total[1] + teamb[1]))
			{
				formatex(ScoreMsg, charsmax(ScoreMsg), "%s vs %s | %d:%d", teamatag, teambtag, total[0] + teama[1],total[1] + teamb[1]);
				ColorChat( 0, BLUE, "%s %L %d-%d", teamatag,total[0] + teama[1], LANG_PLAYER, "MATCH_IS_WINNING",total[1] + teamb[1]);
			}
			else if ((total[0] + teama[1]) < (total[1] + teamb[1]))
			{
				formatex(ScoreMsg, charsmax(ScoreMsg), "%s vs %s | %d:%d", teambtag, teamatag, total[1] + teamb[1],total[0] + teama[1]);
				ColorChat( 0, RED, "%s %L %d-%d", teambtag, total[1] + teamb[1], LANG_PLAYER, "MATCH_IS_WINNING",total[0] + teama[1]);
			}
			else
			{
				formatex(ScoreMsg, charsmax(ScoreMsg), "%s vs %s | %d:%d", teamatag, teambtag, total[1] + teamb[1],total[0] + teama[1]);	
				ColorChat( 0, GREY, "%L %d-%d",total[1] + teamb[1], LANG_PLAYER, "MATCH_IS_TIED",total[0] + teama[1]);
			}
			SetServerName(ScoreMsg);
		}
	}
}

public match_weapon_detect(id)
{
	if(match_get_iskniferound())
	{
		new weaponid = read_data(2);
		if(weaponid != CSW_KNIFE)
			engclient_cmd(id,"weapon_knife");
	}
}

public match_round_start()
{
	if(match_get_iswarm())
	{
		new players[32], num;
		get_players(players, num);
		new i;
		for (i=0; i<num; i++)
		{
			if(cs_get_user_team(players[i]) == CS_TEAM_T || cs_get_user_team(players[i]) == CS_TEAM_CT)
			{
				cs_set_user_money(players[i], 16000);
			}
		}
	}
}

public match_show_teams()
{
		new msgt[512],msgct[512],tops,ctops;
		new teamatag[32],teambtag[32];

		if (get_pcvar_num(match_p_allow_changetag))
		{
			get_pcvar_string(match_p_teamatag, teamatag, charsmax(msgt));
			get_pcvar_string(match_p_teambtag, teambtag, charsmax(msgct));
		}
		else
		{
			teamatag = "Red Team";
			teambtag = "Blue Team";
		}


		format(msgt,charsmax(msgt),"%s^n-------------------------^n",teamatag);
		tops=strlen(msgt);
		format(msgct,charsmax(msgct),"%s^n-------------------------^n",teambtag);
		ctops=strlen(msgct);
		
		for(new i = 0; i < get_maxplayers(); i++)
		{
			if(get_user_team(i) == 1)
			{
				new name[32];
				get_user_name(i,name,31);
				tops+=format(msgt[tops],511-tops,"%s^n",name);
			}
			else  if(get_user_team(i) == 2)
			{
				new name[32];
				get_user_name(i,name,31);
				ctops+=format(msgct[ctops],511-ctops,"%s^n",name);
			}
		}
		set_hudmessage(0, 70, 200, 0.70, 0.41, 0, 0.4, 10.0, 0.08, 2.0, -1);
		ShowSyncHudMsg(0,MATCH_MSG_CT,msgct);
		set_hudmessage(0, 70, 200, 0.70, 0.41, 0, 0.4, 10.0, 0.08, 2.0, -1);
		show_hudmessage(0, "^n^n^n^n^n^n^n-------------------------");
		set_hudmessage(255, 0, 0, 0.20, 0.41, 0, 0.4, 10.0, 0.08, 2.0, -1);
		ShowSyncHudMsg(0,MATCH_MSG_TR,msgt);
		set_hudmessage(255, 0, 0, 0.20, 0.41, 0, 0.4, 10.0, 0.08, 2.0, -1);
		show_hudmessage(0, "^n^n^n^n^n^n^n-------------------------");
}

public match_knife_check_timesup( const MsgId, const MsgDest, const MsgEntity )
{
	if(match_get_iskniferound())
	{
		static message[32];
		get_msg_arg_string(2, message, charsmax(message));
		if(equal(message,"#Target_Saved"))
		{
			set_hudmessage(255, 0, 0, -1.0, 0.43, 0, 0.0, 10.0, 0.0, 0.0, 1);
			show_hudmessage(0, "%L", LANG_PLAYER, "KNIFE_TARGET_SAVED");
			match_stop(1);
			match_start(1);
		}
		else if(equal(message, "#Terrorists_Win"))
		{
			vote_knife_round_win(CS_TEAM_T);
		}
		else if(equal(message, "#CTs_Win"))
		{
			vote_knife_round_win(CS_TEAM_CT);
		}
	}
}

public match_change_team_tag()
{
	if (get_pcvar_num(match_p_allow_changetag) == 1 && !match_get_iskniferound())
	{
		new teamatag[32], teambtag[32], name[128];
		get_pcvar_string(match_p_teamatag, teamatag, charsmax(teamatag));
		get_pcvar_string(match_p_teambtag, teambtag, charsmax(teambtag));
		if(match_get_inmatch() && !match_get_issechalf())
		{
			for(new i = 0; i < get_maxplayers(); i++)
			{
				get_user_name(i, name, charsmax(name));
				if(cs_get_user_team(i) == CS_TEAM_T)
				{
					if ( containi(name, teamatag) == -1 )
					{
						format(name, charsmax(name), "%s %s", teamatag, match_ga_player_orig_name[i]);
						set_user_info(i, "name", name);
					}
				}
				if(cs_get_user_team(i) == CS_TEAM_CT)
				{
					if ( containi(name, teambtag) == -1 )
					{
						format(name, charsmax(name), "%s %s", teambtag, match_ga_player_orig_name[i]);
						set_user_info(i, "name", name);
					}
				}
			}
		}
		else if (match_get_inmatch() && match_get_issechalf())
		{
			for(new i = 0; i < get_maxplayers(); i++)
			{
				get_user_name(i, name, charsmax(name));
				if(cs_get_user_team(i) == CS_TEAM_CT)
				{
					if ( containi(name, teamatag) == -1 )
					{
						format(name, charsmax(name), "%s %s", teamatag, match_ga_player_orig_name[i]);
						set_user_info(i, "name", name);
					}
				}
				if(cs_get_user_team(i) == CS_TEAM_T)
				{
					if ( containi(name, teambtag) == -1 )
					{
						format(name, charsmax(name), "%s %s", teambtag, match_ga_player_orig_name[i]);
						set_user_info(i, "name", name);
					}
				}
			}
		}
	}
}

public match_change_team_tag_back()
{
	if (get_pcvar_num(match_p_allow_changetag) == 1)
	{
		for(new i = 0; i < get_maxplayers(); i++)
		{
			if(!is_user_connected(i) || is_user_hltv(i)) continue;
			set_user_info(i, "name", match_ga_player_orig_name[i]);
		}
	}
}