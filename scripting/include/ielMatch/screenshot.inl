new ss_p_allow_screenshot;

public ss_plugin_init()
{
	ss_p_allow_screenshot = register_cvar("im_allow_screenshot","1");
}

public bool:ss_get_allow()
{
	if(!get_pcvar_num(ss_p_allow_screenshot)) return false;
	return true;
}

public ss_screenshot()
{
	ss_open_scoreboard();
	ShowDirectorMessage(0, 0.60, 0.63, 255, 255, 255, 0, 3.0, 0.0, 0.0, 0.0, "%s v%s^nCopyright (C) 2012 by %s" ,PLUGIN,VERSION,AUTHOR);
	set_task(0.5,"ss_get_screenshot");
	set_task(1.0,"ss_close_scorebarod");
}

public ss_open_scoreboard()
{
	client_cmd(0, "+showscores");
	return PLUGIN_CONTINUE;

}

public ss_close_scorebarod()
{
	client_cmd(0, "-showscores");
	return PLUGIN_CONTINUE;
}

public ss_get_screenshot()
{
	new players[32];
	new i, num;
	get_players(players,num);
	for(i = 0; i < num; i++)
	{
		if(cs_get_user_team(players[i]) == CS_TEAM_T || cs_get_user_team(players[i]) == CS_TEAM_CT) client_cmd(players[i],"snapshot");
	}
	return PLUGIN_CONTINUE;
}