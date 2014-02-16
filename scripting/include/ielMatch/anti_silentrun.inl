#define ASR_VEC_VIEW 17.0
#define ASR_VEC_DUCK_VIEW 12.0


new asr_g_silentrun;
new Float:asr_last_check_time[MAX_CLIENTS + 1];
new Float:asr_g_view_ofs[MAX_CLIENTS + 1][3]

new const asr_g_player_hull[] = {
	HULL_HUMAN,
	HULL_HEAD
}

public asr_plugin_init()
{
	register_forward(FM_PlayerPreThink, "asr_fwPlayerPreThink")
	register_forward(FM_PlayerPostThink, "asr_fwPlayerPostThink")

	asr_g_silentrun = register_cvar("im_antisilentrun", "1");
}

public asr_fwPlayerPreThink(id)
{
	if(!get_pcvar_num(asr_g_silentrun) || !is_user_alive(id) || !(pev(id, pev_button) & IN_DUCK)) return FMRES_IGNORED;
	if((get_gametime() - asr_last_check_time[id]) < 0.055)
	{
		pev(id, pev_view_ofs, asr_g_view_ofs[id]);
		return FMRES_IGNORED;
	}
	asr_last_check_time[id] = get_gametime();
	return FMRES_HANDLED;
}

public asr_fwPlayerPostThink(id)
{
	if(!get_pcvar_num(asr_g_silentrun) || asr_g_view_ofs[id][2] != ASR_VEC_VIEW || !is_user_alive(id) || !(pev(id, pev_button) & IN_DUCK) || pev(id, pev_fuser2)) return FMRES_IGNORED;
	
	pev(id, pev_view_ofs, asr_g_view_ofs[id]);
	if(asr_g_view_ofs[id][2] != ASR_VEC_DUCK_VIEW) return FMRES_IGNORED;
	
	static Float:vec1[3], Float:vec2[3], Float:size_z;
	pev(id, pev_size, vec1);
	size_z = vec1[2];
	pev(id, pev_origin, vec1);
	vec2[0] = vec1[0];
	vec2[1] = vec1[1];
	vec2[2] = -9999.0;
	
	engfunc(EngFunc_TraceHull, vec1, vec2, IGNORE_MONSTERS, asr_g_player_hull[!!(pev(id, pev_flags) & FL_DUCKING)], id, 0);
	get_tr2(0, TR_vecEndPos, vec2);
	if(vec1[2] - vec2[2] > size_z)
		return FMRES_IGNORED;
		
	ShowDirectorMessage(id, -1.0, 0.70, 0, 255, 0, 0, 1.1, 0.2, 0.2, 0.0, "%L", id, "DENY_SILENTRUN");
	pev(id, pev_velocity, vec1);
	vec1[0] = 0.0;
	vec1[1] = 0.0;
	set_pev(id, pev_velocity, vec1);
	
	return FMRES_HANDLED;
}