/*
插件名称：团队语音控制插件
插件原作者：Rulzy
Web URL ：http://bbs.covert.cn/


命令说明：
	im_mute <name or #userid>	静音某玩家麦克风
	im_unmute <name or #userid>	取消静音某玩家麦克风
参数说明：
	im_teamtalk 0|1 	（默认：1）是否开启此插件的语音控制功能
	im_talkhint 0|1|2	（默认：1）提示类型：0-关闭, 1-HUD, 2-聊天区
	im_talktime 整数值 	（默认：0）语音控制的一个时间参数，详细说明见后。

	参数 im_talktime 说明：
		当 im_talktime 为 0 时，队友间总是可以互相听到讲话（当然，要求参数amx_teamtalk不为零，下同）
		当 im_talktime 为正数时，玩家死亡后的amx_talktime秒内，
				活着的队友可以听到其讲话，超过这个时间则活着的队友听不到其讲话了
				主要用于：当玩家刚死亡时，可以给活着的队友报点，又可以避免一直讲话而影响活着的队友发挥
		当 im_talktime 为负数时，玩家死亡后的 |amx_talktime| 秒内，
				活着的队友听不到其讲话，超过这个时间则活着的队友可以听到其讲话
				主要用于：当玩家刚死亡时，避免玩家报点，但时间过后可以通话，可用于战术指挥，训练时感觉比较有用
					
	注意，参数 sv_alltalk 必须为0，否则此插件功能不会生效。
*/

new tt_g_player_muted[33]
new Float: tt_g_player_deathtime[33]
new tt_p_teamtalk, tt_p_talktime, tt_p_talkhint

public tt_plugin_init()
{
	register_forward(FM_Voice_SetClientListening, "tt_fm_voice_forward")
	register_clcmd("im_mute", "tt_cmdMute", ADMIN_KICK, "<name or #userid> - Mute player's Microphone");
	register_clcmd("im_unmute", "tt_cmdUnMute", ADMIN_KICK, "<name or #userid> - UnMute player's Microphone");
	
	tt_p_teamtalk = register_cvar("im_teamtalk", "1");
	tt_p_talktime = register_cvar("im_talktime", "0");
	tt_p_talkhint = register_cvar("im_talkhint", "1");
	
	register_event("DeathMsg", "tt_eDeathMsg", "a");
	
}

public tt_eDeathMsg()
{
	new victim = read_data(2);
	if(!is_user_connected(victim)) return PLUGIN_CONTINUE;
	tt_g_player_deathtime[victim] = get_gametime();
	if(get_cvar_num("sv_alltalk")) return PLUGIN_CONTINUE;
	if(!get_pcvar_num(tt_p_teamtalk)) return PLUGIN_CONTINUE;
	if(tt_g_player_muted[victim] && get_user_flags(victim)&ADMIN_IMMUNITY==0)  //如果此人被禁止讲话
		return PLUGIN_CONTINUE;
	if(!get_pcvar_num(tt_p_talkhint))
		return PLUGIN_CONTINUE;
	new talktime = get_pcvar_num(tt_p_talktime);
	talktime = talktime>0?talktime:-talktime;
	new param[3]
	param[0] = victim;
	param[1] = talktime;
	tt_ShowHint(param);
	return PLUGIN_CONTINUE;
}

public tt_ShowHint(param[])
{
	if(get_cvar_num("sv_alltalk")) return PLUGIN_CONTINUE;
	if(!get_pcvar_num(tt_p_teamtalk)) return PLUGIN_CONTINUE;
	new id = param[0];
	new talktime = get_pcvar_num(tt_p_talktime);
	new timeleft = param[1];
	new HintMsg[256];
	new talkhint = get_pcvar_num(tt_p_talkhint);
	if(talkhint==1)
	{
		if(is_user_alive(id))
		{
			format(HintMsg, 255, "%L", id, "TT_ALIVE_TALK");
			timeleft = 0;
		}else if(talktime==0){
			format(HintMsg, 255, "%L", id, "TT_DEAD_TALK");
		}else if(talktime>0){
			if(timeleft==0)
				format(HintMsg, 255, "%L", id, "TT_DEAD_TALK1", timeleft);
			else
				format(HintMsg, 255, "%L", id, "TT_DEAD_TALK2", timeleft);
		}else{
			if(timeleft==0)
				format(HintMsg, 255, "%L", id, "TT_DEAD_TALK3", timeleft);
			else
				format(HintMsg, 255, "%L", id, "TT_DEAD_TALK4", timeleft);
		}
		set_hudmessage(255, 196, 196, -1.0, 0.73, 0, 0.0, 3.0, 0.0, 0.0, 1)
		show_hudmessage(id, HintMsg)
	}else if (talkhint==2){
		if(is_user_alive(id))
		{
			format(HintMsg, 255, "^x04[iM]^x01%L", id, "TT_ALIVE_TALK");
			timeleft = 0;
		}else if(talktime==0){
			format(HintMsg, 255, "^x04[iM]^x01%L", id, "TT_DEAD_TALK");
		}else if(talktime>0){
			if(timeleft==0)
				format(HintMsg, 255, "^x04[iM]^x01%L", id, "TT_DEAD_TALK1", timeleft);
			else
				format(HintMsg, 255, "^x04[iM]^x01%L", id, "TT_DEAD_TALK2", timeleft);
		}else{
			if(timeleft==0)
				format(HintMsg, 255, "^x04[iM]^x01%L", id, "TT_DEAD_TALK3", timeleft);
			else
				format(HintMsg, 255, "^x04[iM]^x01%L", id, "TT_DEAD_TALK4", timeleft);
		}
		tt_client_color(id, id, HintMsg);
	}
	if(timeleft>0)
	{
		param[1]=timeleft-1;
		set_task(1.0, "tt_ShowHint", 0, param, 2);
	}
	return PLUGIN_CONTINUE;
}

public tt_fm_voice_forward(receiver, sender, listen)
{
	if(receiver==sender) return FMRES_IGNORED;
	new alltalk = get_cvar_num("sv_alltalk");
	if(alltalk) return FMRES_IGNORED;
	if(!get_pcvar_num(tt_p_teamtalk)) return FMRES_IGNORED;
	if(!is_user_connected(receiver) || !is_user_connected(sender))
		return FMRES_IGNORED;
	if(tt_g_player_muted[sender] && get_user_flags(sender)&ADMIN_IMMUNITY==0)  //如果此人被禁止讲话
	{
		engfunc(EngFunc_SetClientListening, receiver, sender, 0);
		return FMRES_SUPERCEDE;
	}
	new CsTeams:rt=cs_get_user_team(receiver);
	new CsTeams:st=cs_get_user_team(sender);
	if(rt==CS_TEAM_UNASSIGNED || rt==CS_TEAM_SPECTATOR)	//观察员可以听到所有人讲话
	{
		engfunc(EngFunc_SetClientListening, receiver, sender, 1);
		return FMRES_SUPERCEDE;
	}
	if(rt==st){  //如果是队友间讲话
		if(is_user_alive(sender))  //活着的队友讲话，队友都可以听到
		{
			engfunc(EngFunc_SetClientListening, receiver, sender, 1);
			return FMRES_SUPERCEDE;
		}
		//死亡的队友讲话，分两种情况
		if(!is_user_alive(receiver))  //一：死亡的队友可以听到死亡的队友讲话(包括两者都是观察员的情况)
		{
			engfunc(EngFunc_SetClientListening, receiver, sender, 1); 
			return FMRES_SUPERCEDE;
		}
		//二：如果讲话者是死的，听话者是活的，则根据参数和死亡时间来区分
		new talktime = get_pcvar_num(tt_p_talktime);
		if(talktime == 0)
		{
			engfunc(EngFunc_SetClientListening, receiver, sender, 1); //总可以听到队友讲话
			return FMRES_SUPERCEDE;
		}
		new Float:gametime = get_gametime();
		if(talktime>0){  //队友死亡后多少秒内活着的队友可以听到，可用于报点，但可防止时间过后影响活着的队友发挥
			if(gametime-tt_g_player_deathtime[sender]>talktime)//如果当前时间-最后死亡时间>指定的时间，则活的队友听不到讲话
			{
				engfunc(EngFunc_SetClientListening, receiver, sender, 0);
			}else{							//否则（当前时间-最后死亡时间<=指定的时间），活的队友可听到讲话
				engfunc(EngFunc_SetClientListening, receiver, sender, 1);
			}
		}else{  //队友死亡后多少秒内活着的队友听不到，可防止报点，但这个时间过后可以进行战术指挥
			talktime = - talktime;  //取绝对值
			if(gametime-tt_g_player_deathtime[sender]>talktime)//如果当前时间-最后死亡时间>指定的时间，则活的队友可听到讲话
			{
				engfunc(EngFunc_SetClientListening, receiver, sender, 1);
			}else{							//否则（当前时间-最后死亡时间<=指定的时间），活的队友听不到讲话
				engfunc(EngFunc_SetClientListening, receiver, sender, 0);
			}
		}
	}else{  //否则听不到讲话（不是队友，也不是观察员）
		engfunc(EngFunc_SetClientListening, receiver, sender, 0);
	}
	
	return FMRES_SUPERCEDE;
}

public tt_client_connect(id)
{
	tt_g_player_muted[id] = 0;
	tt_g_player_deathtime[id] = get_gametime();
}

public tt_cmdMute(id, level, cid)
{
	if(!cmd_access(id, level, cid, 2))
		return PLUGIN_HANDLED;
	new name[32];
	read_argv(1, name, 31);
	new target = cmd_target(id, name, 1);
	if(!target) return PLUGIN_HANDLED;
	tt_g_player_muted[target] = 1
	return PLUGIN_HANDLED;
}

public tt_cmdUnMute(id, level, cid)
{
	if(!cmd_access(id, level, cid, 2))
		return PLUGIN_HANDLED;
	new name[32];
	read_argv(1, name, 31);
	new target = cmd_target(id, name, 0);
	if(!target) return PLUGIN_HANDLED;
	tt_g_player_muted[target] = 0
	return PLUGIN_HANDLED;
}

stock tt_client_color(playerid,colorid,msg[])
{
	message_begin(playerid?MSG_ONE:MSG_ALL, get_user_msgid("SayText"), {0,0,0}, playerid) 
	write_byte(colorid)
	write_string(msg)
	message_end()
}