new bool:match_knife_round;
new bool:match_inmatch;
new bool:match_issechalf;
new bool:match_isovertime;
new bool:match_iswarm;

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

public match_start(match_type)
{
	switch(match_type)
	{
		case 1:
		{
			//Knife Round
			match_set_inmatch(true);
			match_set_iskniferound(true);
			match_set_issechalf(false);
			match_set_isot(false);
			match_set_iswarm(false);
			match_r3(match_type);
		}
		case 2:
		{
			//OverTime Round
			match_set_inmatch(true);
			match_set_iskniferound(false);
			match_set_issechalf(false);
			match_set_isot(true);
			match_set_iswarm(false);
			match_r3(match_type);
		}
		case 3:
		{
			//OverTime Second Half
			
			match_set_inmatch(true);
			match_set_iskniferound(false);
			match_set_issechalf(true);
			match_set_isot(true);
			match_set_iswarm(false);
			match_r3(match_type);
		}
		case 4:
		{
			//Common Round
			match_set_inmatch(true);
			match_set_iskniferound(false);
			match_set_issechalf(false);
			match_set_isot(false);
			match_set_iswarm(false);
			match_r3(match_type);
		}
		case 5:
		{
			//Common Second Half
			match_set_inmatch(true);
			match_set_iskniferound(false);
			match_set_issechalf(true);
			match_set_isot(false);
			match_set_iswarm(false);
			match_r3(match_type);
		}
		default:
		{
			//Warmup Round
			match_set_inmatch(false);
			match_set_iskniferound(false);
			match_set_issechalf(false);
			match_set_isot(false);
			match_set_iswarm(true);
			match_r3(match_type);
		}
	}
}


public match_r3(match_type)
{
	match_load_config(match_type);
	switch(match_type)
	{
		case 1:
		{
			new args[1];
			args[0] = 1;
			ColorChat( 0, GREY, "^x04%s^x03%L",Export[MATCHNAME], LANG_PLAYER, "KNIFE_R3");
			set_task(2.0,"match_restart", _, args);
			set_task(2.0,"match_restart", _, args);
			set_task(2.0,"match_restart", _, args);
			ColorChat( 0, GREY, "^x04%s^x03%L",Export[MATCHNAME], LANG_PLAYER, "KNIFE_ROUND");
			ColorChat( 0, GREY, "^x04%s^x03%L",Export[MATCHNAME], LANG_PLAYER, "KNIFE_ROUND");
			ColorChat( 0, GREY, "^x04%s^x03%L",Export[MATCHNAME], LANG_PLAYER, "KNIFE_ROUND");
			ColorChat( 0, GREY, "^x04%s^x03%L",Export[MATCHNAME], LANG_PLAYER, "KNIFE_ROUND");
			ColorChat( 0, GREY, "^x04%s^x03%L",Export[MATCHNAME], LANG_PLAYER, "KNIFE_ROUND");
			ShowDirectorMessage(0, -1.0, 0.30, 0, 200, 0, 0, 0.8, 0.8, 6.0, 2.0, "--[ :: !! KNIFE ROUND !! :: ]--^n--[ :: !! KNIFE ROUND !! :: ]--^n--[ :: !! KNIFE ROUND !! :: ]--");
		}
		case 2:
		{
			
		}
		case 3:
		{
			
		}
		case 4:
		{
			
		}
		default:
		{
			
		}
	}
}

public match_load_config(match_type)
{
	new customdir[64];
	get_customdir(customdir, 63);
	switch(match_type)
	{
		case 1:
		{
			server_cmd("exec ^"%s/ielMatch/match_knife_config.cfg^"", customdir);
		}
		case 2:
		{
			server_cmd("exec ^"%s/ielMatch/match_ot_config.cfg^"", customdir);
		}
		case 3:
		{
			server_cmd("exec ^"%s/ielMatch/match_ot_config.cfg^"", customdir);
		}
		case 4:
		{
			server_cmd("exec ^"%s/ielMatch/match_config.cfg^"", customdir);
		}
		case 5:
		{
			server_cmd("exec ^"%s/ielMatch/match_config.cfg^"", customdir);
		}
		default:
		{
			server_cmd("exec ^"%s/ielMatch/warmup_config.cfg^"", customdir);
		}
	}
}

public match_restart(time[])
{
	server_cmd("sv_restartround %d", time[0]);
}