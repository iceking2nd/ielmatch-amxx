enum cc_Color
{
	NORMAL = 1, // clients scr_concolor cvar color
	GREEN, // Green Color
	TEAM_COLOR, // Red, grey, blue
	GREY, // grey
	RED, // Red
	BLUE, // Blue
}

new cc_TeamName[][] = 
{
	"",
	"TERRORIST",
	"CT",
	"SPECTATOR"
}

stock ShowDirectorMessage(id, Float:x, Float:y, r, g, b, effect, Float:fadeintime, Float:fadeouttime, Float:holdtime, Float:fxtime, msg[], {Float, Sql, Result, _}:...)
{
	new text[128];
	vformat(text, charsmax(text), msg, 13);
	new len = strlen(text);
	
	if(!len) return;
	if(id)
		message_begin(MSG_ONE_UNRELIABLE, SVC_DIRECTOR, _, id);
	else
		message_begin(MSG_BROADCAST, SVC_DIRECTOR, _, 0);
	
	write_byte(31+len);	// command length
	write_byte(DRC_CMD_MESSAGE);		// command_event
	write_byte(effect);		// effect
	write_byte(b);	// b
	write_byte(g);	// g
	write_byte(r);	// r
	write_byte(0);	// a
	write_long(_:x);		// x
	write_long(_:y);		// y
	write_long(_:fadeintime);		// fade in time
	write_long(_:fadeouttime);		// fade out time
	write_long(_:holdtime);	// hold time
	write_long(_:fxtime);	// [optional] effect time - time the highlight lags behing the leading text in effect 2
	write_string(text);	// string text message
	message_end();
}

ColorChat(id, cc_Color:type, const msg[], {Float,Sql,Result,_}:...)
{
	new message[256];

	switch(type)
	{
		case NORMAL: // clients scr_concolor cvar color
		{
			message[0] = 0x01;
		}
		case GREEN: // Green
		{
			message[0] = 0x04;
		}
		default: // White, Red, Blue
		{
			message[0] = 0x03;
		}
	}

	vformat(message[1], 251, msg, 4);

	// Make sure message is not longer than 192 character. Will crash the server.
	message[192] = '^0';

	new team, ColorChange, index, MSG_Type;
	
	if(id)
	{
		MSG_Type = MSG_ONE;
		index = id;
	} else {
		index = cc_FindPlayer();
		MSG_Type = MSG_ALL;
	}
	
	team = get_user_team(index);
	ColorChange = cc_ColorSelection(index, MSG_Type, type);

	cc_ShowColorMessage(index, MSG_Type, message);
		
	if(ColorChange)
	{
		cc_Team_Info(index, MSG_Type, cc_TeamName[team]);
	}
}

cc_FindPlayer()
{
	new i = -1;

	while(i <= get_maxplayers())
	{
		if(is_user_connected(++i))
			return i;
	}

	return -1;
}

cc_ColorSelection(index, type, cc_Color:Type)
{
	switch(Type)
	{
		case RED:
		{
			return cc_Team_Info(index, type, cc_TeamName[1]);
		}
		case BLUE:
		{
			return cc_Team_Info(index, type, cc_TeamName[2]);
		}
		case GREY:
		{
			return cc_Team_Info(index, type, cc_TeamName[0]);
		}
	}

	return 0;
}

cc_ShowColorMessage(id, type, message[])
{
	static bool:saytext_used;
	static get_user_msgid_saytext;
	if(!saytext_used)
	{
		get_user_msgid_saytext = get_user_msgid("SayText");
		saytext_used = true;
	}
	message_begin(type, get_user_msgid_saytext, _, id);
	write_byte(id)		
	write_string(message);
	message_end();	
}

cc_Team_Info(id, type, team[])
{
	static bool:teaminfo_used;
	static get_user_msgid_teaminfo;
	if(!teaminfo_used)
	{
		get_user_msgid_teaminfo = get_user_msgid("TeamInfo");
		teaminfo_used = true;
	}
	message_begin(type, get_user_msgid_teaminfo, _, id);
	write_byte(id);
	write_string(team);
	message_end();

	return 1;
}