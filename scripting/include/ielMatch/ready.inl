new rdy_player_isready[33];
new rdy_player_kick_timeleft[33];
new rdy_player_ready_num;
new rdy_p_readytimeleft;
new rdy_p_kniferound;

public rdy_plugin_init()
{
	rdy_p_readytimeleft = register_cvar("im_readytimeleft", "90");
	rdy_p_kniferound = register_cvar("im_kniferound", "1");

	register_clcmd("say ready", "rdy_player_ready");
	register_clcmd("say !ready", "rdy_player_ready");
	register_clcmd("say rdy", "rdy_player_ready");
	register_clcmd("say .r", "rdy_player_ready");
	register_clcmd("say notready", "rdy_player_unready");
	register_clcmd("say unready", "rdy_player_unready");
	register_clcmd("say !notready", "rdy_player_unready");
	register_clcmd("say unrdy", "rdy_player_unready");
	register_clcmd("say .ur", "rdy_player_unready");

}

public rdy_set_task()
{
	set_task(1.0, "rdy_check_task", _, _, _, "b");
}

public rdy_check_task(id)
{
	if(!match_get_inmatch())
	{
		new nreadymsg[512],nreadypos;
	
		nreadymsg = "%L^n";
			
		nreadypos = strlen(nreadymsg);
		new players[32];
		new i, num;
		get_players(players,num);
		for(i = 0; i < num; i++)
		{
			if((cs_get_user_team(players[i]) == CS_TEAM_T || cs_get_user_team(players[i]) == CS_TEAM_CT) && !rdy_player_isready[players[i]] )
			{
				new nReadyname[32];
				get_user_name(players[i],nReadyname,31);
				nreadypos += format(nreadymsg[nreadypos],511-nreadypos,"%s^n",nReadyname);
				rdy_player_kick_timeleft[players[i]]++;
				client_print(players[i], print_center, "%L", players[i], "READY_TIME_LEFT", get_pcvar_num(rdy_p_readytimeleft) - rdy_player_kick_timeleft[players[i]]);
				if (rdy_player_kick_timeleft[players[i]] >= get_pcvar_num(rdy_p_readytimeleft)) rdy_kick_player(players[i]);
			}
			else
			{
				continue;
			}
		}
		ShowDirectorMessage(id, 0.15, 0.30, 255, 255, 255, 0, 0.1, 6.0, 0.0, 0.0, nreadymsg, LANG_PLAYER, "NEED_NUMBER", 10 - rdy_player_ready_num);
		ShowDirectorMessage(id, 0.61, 0.30, 255, 255, 255, 0, 0.1, 6.0, 0.0, 0.0, "%L", LANG_PLAYER, "DASHBOARD_MSG1");
		ShowDirectorMessage(id, 0.61, 0.43, 255, 255, 255, 0, 0.1, 6.0, 0.0, 0.0, "%L", LANG_PLAYER, "DASHBOARD_MSG2");
		ShowDirectorMessage(id, 0.61, 0.56, 255, 255, 255, 0, 0.1, 6.0, 0.0, 0.0, "%L", LANG_PLAYER, "DASHBOARD_MSG3");
		ShowDirectorMessage(id, 0.61, 0.70, 255, 255, 255, 0, 0.1, 6.0, 0.0, 0.0, "%s v%s^nCopyright (C) 2014 by %s" ,PLUGIN,VERSION,AUTHOR);

		if (rdy_player_ready_num >= 10)
		{
			if(get_pcvar_num(rdy_p_kniferound) == 1) match_start(1);
			else match_start(4);
		}

	}
}

public rdy_kick_player(id)
{
	new name[32];
	get_user_name(id, name, 31);
	new uID = get_user_userid(id);
//	server_cmd("banid 1 #%d", uID);
	client_cmd(id, "echo ^"[iM] %L^"", id, "NOT_READY_KICK1");
	server_cmd("kick #%d [iM] %L",uID, id, "NOT_READY_KICK1");
	client_print(0, print_chat, "[iM] %s %L", name, LANG_PLAYER, "NOT_READY_KICK2");
}

public rdy_player_ready(id)
{
	if(cs_get_user_team(id) == CS_TEAM_SPECTATOR)
	{
		ColorChat(id, TEAM_COLOR, "^x01%L", id, "SPEC_READY");
		return PLUGIN_HANDLED;
	}
	if(match_get_inmatch())
	{
		ColorChat(id, TEAM_COLOR, "^x01%L", id, "MATCH_READY");
		return PLUGIN_HANDLED;
	}
	if(rdy_player_isready[id])
	{
		ColorChat(id, TEAM_COLOR, "^x01%L", id, "MATCH_ALREADY");
		return PLUGIN_HANDLED;
	}
	new rName[32];
	get_user_name(id,rName,31);
	ColorChat(id, TEAM_COLOR, "%s ^x01%L", rName, id, "READY");
	rdy_player_isready[id] = true;
	rdy_player_ready_num++;

	return PLUGIN_HANDLED;
}

public rdy_player_unready(id)
{
	if(cs_get_user_team(id) == CS_TEAM_SPECTATOR)
	{
		ColorChat(id, TEAM_COLOR, "^x01%L", id, "SPEC_NOTREADY");
		return PLUGIN_HANDLED;
	}
	if(match_get_inmatch())
	{
		ColorChat(id, TEAM_COLOR, "^x01%L", id, "MATCH_NOTREADY");
		return PLUGIN_HANDLED;
	}
	if(!rdy_player_isready[id])
	{
		ColorChat(id, TEAM_COLOR, "^x01%L", id, "MATCH_NOTALREADY");
		return PLUGIN_HANDLED;
	}
	new rName[32];
	get_user_name(id,rName,31);
	ColorChat(id, TEAM_COLOR, "%s ^x01%L", rName, id, "NOTREADY");
	rdy_player_isready[id] = false;
	rdy_player_ready_num--;
	return PLUGIN_HANDLED;
}

public rdy_client_disconnect(id)
{
	if(rdy_player_isready[id])
	{
		rdy_player_kick_timeleft[id] = 0;
		rdy_player_isready[id] = false;
		rdy_player_ready_num--;
	}
}
