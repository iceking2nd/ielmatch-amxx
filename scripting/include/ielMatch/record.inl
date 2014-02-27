new rec_p_recpov, rec_p_rechltv;
new rec_p_allow_hltv_prefix, rec_p_hltv_prefix;
new rec_p_hltv_password;
new rec_hltv_id, rec_hltv_ip[32], rec_hltv_port;
new rec_p_hltv_delay;


public rec_plugin_init()
{
	rec_p_recpov = register_cvar("im_recpov","1");
	rec_p_rechltv = register_cvar("im_rechltv","1");
	rec_p_allow_hltv_prefix = register_cvar("im_allow_hltv_prefix","1");
	rec_p_hltv_prefix = register_cvar("im_hltv_prefix","iM");
	rec_p_hltv_password = register_cvar("im_hltv_password","");
	rec_p_hltv_delay = register_cvar("im_hltv_delay", "30");
}

public rec_client_putinserver(id)
{
	if(is_user_hltv(id))
	{
		new command[512], left[32], right[32];
		rec_hltv_id = id;
		get_user_ip(rec_hltv_id, rec_hltv_ip, charsmax(rec_hltv_ip));
		strtok (rec_hltv_ip, left, charsmax(left), right, charsmax(right), ':');
		copy(rec_hltv_ip, 31, left);
		rec_hltv_port = str_to_num(right);
		formatex(command, charsmax(command), "say HLTV CONNECTED");
		hltv_rcon_command(command, 0);
		format(command, 255, "delay 0");
		hltv_rcon_command(command, 0);
	}
}

public rec_pov_demo_start()
{
	if(get_pcvar_num(rec_p_recpov))
	{
		new mapname[32];
		new players[32];
		get_mapname(mapname, 31);
		new demo_name[256];
		new thetime[32];
		get_time("%Y.%m.%d-%H.%M.%S",thetime,31);
		new i, num;
		get_players(players,num);
		for(i = 0; i < num; i++)
		{
			if(!is_user_connected(players[i]) || is_user_hltv(players[i])) continue;
			if(cs_get_user_team(players[i]) == CS_TEAM_SPECTATOR) continue;
			if(cs_get_user_team(players[i]) == CS_TEAM_T)
				format(demo_name,255,"%s_%s_%s.dem", match_ga_player_orig_name[players[i]],mapname,thetime);
			if(cs_get_user_team(players[i]) == CS_TEAM_CT)
				format(demo_name,255,"%s_%s_%s.dem", match_ga_player_orig_name[players[i]],mapname,thetime);
			client_cmd(players[i],"stop");
			while(replace(demo_name,255,"/","-")) {}
			while(replace(demo_name,255,"\","-")) {}
			while(replace(demo_name,255,":","-")) {}
			while(replace(demo_name,255,"*","-")) {}
			while(replace(demo_name,255,"?","-")) {}
			while(replace(demo_name,255,">","-")) {}
			while(replace(demo_name,255,"<","-")) {}
			while(replace(demo_name,255,"|","-")) {}
			while(replace(demo_name,255," ","-")) {}
			ColorChat(players[i], TEAM_COLOR, "^x04%s^x03%L %s。",Export[MATCHNAME], players[i], "REC_POV_FILENAME",demo_name);
			client_cmd(players[i],"record ^"%s.a^"",demo_name);
		}
	}
}

public rec_pov_demo_stop(id)
{
	if(get_pcvar_num(rec_p_recpov))
	{
		client_cmd(id, "stop");
		ColorChat(id, TEAM_COLOR, "%L", id, "REC_POV_STOP");
	}				
}

public rec_hltv_demo_start(id,level,cid)
{
	if(get_pcvar_num(rec_p_rechltv))
	{
		new hltv_demo_name[256];
		new hltv_command[512];
		new hltvdemoprefix[16];
		get_pcvar_string(rec_p_hltv_prefix, hltvdemoprefix, charsmax(hltvdemoprefix));
		hltv_rcon_command("stoprecording", 0);
		if(get_pcvar_num(rec_p_allow_hltv_prefix)) formatex(hltv_demo_name, charsmax(hltv_demo_name), "%s-HLTV-Record", hltvdemoprefix);
		else formatex(hltv_demo_name, charsmax(hltv_demo_name), "HLTV-Record");
		while(replace(hltv_demo_name,255,"/","-")) {}
		while(replace(hltv_demo_name,255,"\","-")) {}
		while(replace(hltv_demo_name,255,":","-")) {}
		while(replace(hltv_demo_name,255,"*","-")) {}
		while(replace(hltv_demo_name,255,"?","-")) {}
		while(replace(hltv_demo_name,255,">","-")) {}
		while(replace(hltv_demo_name,255,"<","-")) {}
		while(replace(hltv_demo_name,255,"|","-")) {}
		while(replace(hltv_demo_name,255," ","-")) {}
		formatex(hltv_command, 511, "say RECORDING");
		hltv_rcon_command(hltv_command, 0);
		format(hltv_command, 511, "record %s", hltv_demo_name);
		hltv_rcon_command(hltv_command, 0);
	}
	return PLUGIN_CONTINUE	;
}

public rec_hltv_demo_stop()
{
	if(get_pcvar_num(rec_p_rechltv))
	{
		set_task(get_pcvar_float(rec_p_hltv_delay),"rec_hltv_demo_stop_handle");
	}
}

public hltv_rcon_command(hltv_command[], id)
{
	new socket_address;
	new socket_error = 0;
	
	new receive[256];
	new send[256];
	new hltv_password[64];
	
	new hltv_challenge[13];
	
	if (rec_hltv_id == 0)
	{
		client_print(id, print_console, "HLTV not connect");
	}
	else
	{
		socket_address = socket_open(rec_hltv_ip, rec_hltv_port, SOCKET_UDP, socket_error);
		
		if (socket_error != 0)
		{
			client_print(id, print_console, "HLTV Connect ERROR %i", socket_error);
			return PLUGIN_CONTINUE;
		}
		setc(send, 4, 0xff);
		copy(send[4], 255, "challenge rcon");
		setc(send[18], 1, '^n');
		socket_send(socket_address, send, 255);
		socket_recv(socket_address, receive, 255);
		
		copy(hltv_challenge, 12, receive[19]);
		replace(hltv_challenge, 255, "^n", "");
		setc(send, 255, 0x00);
		setc(send, 4, 0xff);
		
		get_pcvar_string(rec_p_hltv_password, hltv_password, charsmax(hltv_password));
		
		formatex(send[4],255,"rcon %s %s %s^n",hltv_challenge, hltv_password, hltv_command);
		socket_send(socket_address, send, 255);
		socket_close(socket_address);
	}
	return PLUGIN_CONTINUE;
}

public rec_hltv_demo_stop_handle()
{
	hltv_rcon_command("echo hltv stop recording.", 0);
	hltv_rcon_command("stoprecording", 0);
	hltv_rcon_command("say hltv stop recording.", 0);
}