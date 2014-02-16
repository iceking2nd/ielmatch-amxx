#define TM_OFFSET_MONEY	115
#define TM_EXTRAOFFSET	5

new bool:tm_RoundShowMoney;
new tm_g_showmoney, tm_MONEY_HUD;

public tm_plugin_init()
{
	tm_g_showmoney = register_cvar("im_showmoney", "1");

	tm_MONEY_HUD = CreateHudSyncObj();
}

public tm_plugin_cfg()
{
	tm_RoundShowMoney = false;
}

public tm_end_round()
{
	tm_RoundShowMoney = true;
}

public tm_restart_round()
{
	tm_RoundShowMoney = false;
}

public tm_FwdPlayerSpawn(id)
{
	if(!is_user_connected(id)) return HAM_IGNORED;
	if(!tm_RoundShowMoney) return HAM_IGNORED;
	
	switch(get_pcvar_num(tm_g_showmoney))
	{
		case 1: set_task(0.2, "tm_SHOW_MONEY_TASK_HUD",id);
		case 2: set_task(0.2, "tm_SHOW_MONEY_TASK_TEXT",id);
		default: return HAM_IGNORED;
	}

	return HAM_IGNORED;
}

public tm_SHOW_MONEY_TASK_HUD(id)
{
	new pos,opmsg[512];
	static i;
	opmsg="%L^n";
	pos = strlen(opmsg);
	for(i = 1; i <= get_maxplayers(); i++)
	{
		if(get_user_team(i) == get_user_team(id))
		{
			new username[32];
			new iMoney = fm_cs_get_user_money(i);
			get_user_name(i,username,31);
			pos+=format(opmsg[pos],511-pos,"%s ($%d)^n",username,iMoney);
		}
	}
	set_hudmessage (50, 100, 0, 0.01, 0.25, 0, 0.5, 8.0, 0.08, 2.0);
	ShowSyncHudMsg(id,tm_MONEY_HUD,opmsg, LANG_PLAYER, "TEAMMATES_MONEY");
}

public tm_SHOW_MONEY_TASK_TEXT(id)
{
	new MyMoney = fm_cs_get_user_money(id);
	new MoneyMsg[16];
	format(MoneyMsg,15,"$ %d",MyMoney);
	engclient_cmd(id,"say_team",MoneyMsg);
}

stock fm_cs_get_user_money(client)
	return get_pdata_int(client, TM_OFFSET_MONEY, TM_EXTRAOFFSET)