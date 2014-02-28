#define TEAM_SELECT_VGUI_MENU_ID 2
#define Keysmenu_1 (1<<0)|(1<<1)|(1<<4)|(1<<5)|(1<<9) // Keys: 12560
#define Keysmenu_2 (1<<0)|(1<<1)|(1<<4)|(1<<5)|(1<<9) // Keys: 12560

new bool:g_unassigned[33]
new bool:g_changed[33]
new g_msgid[33]
new p_allow_spectators
new bool:g_allow_t
new bool:g_allow_ct
new bool:g_allow_spec
new g_num_t, g_num_ct

public model_plugin_init()
{
	register_message(get_user_msgid("ShowMenu"), "model_msg_show_menu");
	register_message(get_user_msgid("VGUIMenu"), "model_msg_vgui_menu");
	register_menucmd(register_menuid("mainmenu"), Keysmenu_1, "_menu_chooseteam");
	register_clcmd("chooseteam", "hook_chooseteam");

	p_allow_spectators = get_cvar_pointer("allow_spectators");
}

public model_client_connect(id)
{
	g_changed[id] = false;
	g_unassigned[id] = true;
}

public model_client_disconnect(id)
{
	g_changed[id] = false;
	g_unassigned[id] = true;
}

public model_msg_show_menu(msgid, dest, id) {
	static team_select[] = "#Team_Select"
	static menu_text_code[sizeof team_select]
	get_msg_arg_string(4, menu_text_code, sizeof menu_text_code - 1)
	if (!equal(menu_text_code, team_select))
		return PLUGIN_CONTINUE
	g_msgid[id] = msgid
	menu_chooseteam(id)
	return PLUGIN_HANDLED
}

public model_msg_vgui_menu(msgid, dest, id) {
	if (get_msg_arg_int(1) != TEAM_SELECT_VGUI_MENU_ID)
		return PLUGIN_CONTINUE
	g_msgid[id] = msgid
	menu_chooseteam(id)
	return PLUGIN_HANDLED
}

public menu_chooseteam(id)
{
	if (is_user_connected(id) && !is_user_bot(id) && !is_user_hltv(id))
	{
		model_check_teams(id);
		new text[512], len;
		len = format(text, 511,"Team Select^n^n");
		if (g_changed[id])
		{
			len += format(text[len], 511 - len, "\w>>Only one change was permit^n")
			len += format(text[len], 511 - len, "\d1. Terrorists^n")
			len += format(text[len], 511 - len, "\d2. Counter-Terrorists^n")
			len += format(text[len], 511 - len, "\d5. Auto Select^n") 
			len += format(text[len], 511 - len, "\d6. Spectators^n^n")
		}
		else
		{
			len += format(text[len], 511 - len, "\r>>Select the team to join::^n")
			if ( get_user_nums(1) < 5 ) len += format(text[len], 511 - len, "\r1. \wTerrorists^n")
			if ( get_user_nums(2) < 5 ) len += format(text[len], 511 - len, "\r2. \wCounter-Terrorists^n")
			if ( get_user_nums(1) < 5 && get_user_nums(2) < 5 ) len += format(text[len], 511 - len, "\r5. \wAuto Select^n")
			
		}
		if ((g_allow_spec && !g_changed[id]))
		{
			len += format(text[len], 511 - len, "\r6. \wSpectators^n^n")
			len += format(text[len], 511 - len, "^n\r0. Cancel^n^n")
			show_menu(id, Keysmenu_1, text, -1, "mainmenu")
		}
		else
		{
			len += format(text[len], 511 - len, "^n\r0. Cancel^n^n")
			show_menu(id, Keysmenu_2, text, -1, "mainmenu")
		}
	}
}

public model_check_teams(id)
{
	g_allow_t= true
	g_allow_ct = true
	new playerst[32], playersct[32]
	get_players(playerst, g_num_t, "eh", "TERRORIST")
	get_players(playersct, g_num_ct, "eh", "CT")
	if (get_pcvar_num(p_allow_spectators) && (g_num_t + g_num_ct > 1 || g_unassigned[id]))
	{
		g_allow_spec = true
	}
	else
	{
		g_allow_spec = false
	}
	if (get_user_team(id) == 2)//ct
	{
		g_num_ct--
	}
	else if (get_user_team(id) == 1)//t
	{
		g_num_t--
	}
	new limit = 5
	if (g_num_t >= limit)
	{
		g_allow_t = false
	}
	else if (g_num_ct >= limit)
	{
		g_allow_ct = false
	}
}

public _menu_chooseteam(id, key)
{
	model_check_teams(id)
	switch(key)
	{
		case 0:
		{
			if (g_allow_t && !g_changed[id])
			{
				if ( get_user_nums(1) < 5 )
				{
					team_join(id, "1")
					menu_choosemodel_t(id)
				}
				else
				{
					client_print(id, print_center, "Terrorists Team was full.Can't join in.");
				}
			}
			
		}
		case 1:
		{
			if (g_allow_ct && !g_changed[id])
			{
				if ( get_user_nums(2) < 5 )
				{
					team_join(id, "2")
					menu_choosemodel_ct(id)
				}
				else
				{
					client_print(id, print_center, "Counter-Terrorists Team was full.Can't join in.");
				}
			}
		}
		case 4:
		{
			if (!g_changed[id])
			{
				if ( get_user_nums(1) < 5 && get_user_nums(2) < 5 )
				{
					if (g_allow_t && g_allow_ct)
					{
						if (get_systime() % 2)
						{
							team_join(id, "1")
							menu_choosemodel_t(id)
						}
						else
						{
							team_join(id, "2")
							menu_choosemodel_ct(id)
						}
					}
					else if (g_allow_t)
					{
						team_join(id, "1")
						menu_choosemodel_t(id)
					}
					else if (g_allow_ct)
					{
						team_join(id, "2")
						menu_choosemodel_ct(id)
					}
				}
				else
				{
					client_print(id, print_center, "Both Team was full.Can't join in.");
				}
			}
		}
		case 5:
		{
			if (!g_changed[id])
			{
				force_team_join(id, g_msgid[id], "6", _)
			}
		}
		case 9:
		{
			return PLUGIN_HANDLED;
		}
	}
	return PLUGIN_HANDLED;
}

stock force_team_join(id, menu_msgid, team[] = "5",  class[] = "0") {
	static jointeam[] = "jointeam"
	if (class[0] == '0') {
		engclient_cmd(id, jointeam, team)
		g_changed[id] = true
		g_unassigned[id] = false
		return
	}
	static msg_block, joinclass[] = "joinclass"
	msg_block = get_msg_block(menu_msgid)
	set_msg_block(menu_msgid, BLOCK_SET)
	engclient_cmd(id, jointeam, team)
	engclient_cmd(id, joinclass, class)
	set_msg_block(menu_msgid, msg_block)
	g_changed[id] = true
	g_unassigned[id] = false
}

public hook_chooseteam(id)
{
	menu_chooseteam(id)
	return PLUGIN_HANDLED
}

stock team_join(id, team[] = "5")
{
	new menu_msgid = g_msgid[id]
	new msg_block = get_msg_block(menu_msgid)
	set_msg_block(menu_msgid, BLOCK_SET)
	engclient_cmd(id, "jointeam", team)
	set_msg_block(menu_msgid, msg_block)
}

public menu_choosemodel_t(id)
{
	new menu = menu_create("Model Select^n^n\r>>Default T's Models::", "_menu_choosemodel_t")
	menu_additem(menu, "Phoenix Connexion", "1", 0);
	menu_additem(menu, "Elite Crew", "2", 0);
	menu_additem(menu, "Arctic Avengers", "3", 0);
	menu_additem(menu, "Guerilla Warfare", "4", 0);
	menu_additem(menu, "\wAuto Select", "5", 0);
	menu_setprop(menu, MPROP_EXITNAME, "\rCancle");
	menu_display(id, menu, 0);
}
public _menu_choosemodel_t(id, menu, item)
{
	new data[6], iName[64];
	new access, callback;
	menu_item_getinfo(menu, item, access, data,5, iName, 63, callback);
	new menu_msgid = g_msgid[id]
	new msg_block = get_msg_block(menu_msgid)
	set_msg_block(menu_msgid, BLOCK_SET)
	engclient_cmd(id, "joinclass", data)
	set_msg_block(menu_msgid, msg_block)
	g_changed[id] = true
	g_unassigned[id] = false
	menu_destroy(menu);

	if (rdy_player_isready[id])
	{
		rdy_player_ready_num--;
		rdy_player_isready[id] = false;
	}

	return PLUGIN_HANDLED;
}

public menu_choosemodel_ct(id)
{
	new menu = menu_create("Model Select^n^n\r>>Default CT's Models::", "_menu_choosemodel_ct")
	menu_additem(menu, "Seal Team 6", "1", 0);
	menu_additem(menu, "GSG-9", "2", 0);
	menu_additem(menu, "SAS", "3", 0);
	menu_additem(menu, "GIGN", "4", 0);
	menu_additem(menu, "\wAuto Select", "5", 0);
	menu_setprop(menu, MPROP_EXITNAME, "\rCancel");
	menu_display(id, menu, 0);
}
public _menu_choosemodel_ct(id, menu, item)
{
	new data[6], iName[64];
	new access, callback;
	menu_item_getinfo(menu, item, access, data,5, iName, 63, callback);
	new menu_msgid = g_msgid[id]
	new msg_block = get_msg_block(menu_msgid)
	set_msg_block(menu_msgid, BLOCK_SET)
	engclient_cmd(id, "joinclass", data)
	set_msg_block(menu_msgid, msg_block)
	g_changed[id] = true
	g_unassigned[id] = false
	menu_destroy(menu);

	if (rdy_player_isready[id])
	{
		rdy_player_ready_num--;
		rdy_player_isready[id] = false;
	}

	return PLUGIN_HANDLED;
}
