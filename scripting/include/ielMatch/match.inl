new bool:match_knife_round;
new bool:match_inmatch;
new bool:match_issechalf;
new bool:match_isovertime;

public match_get_inmatch() return match_inmatch;
public match_get_iskniferound() return match_knife_round;
public match_get_issechalf() return match_issechalf;
public match_get_isot() return match_isovertime;
public match_set_inmatch(bool:val) { match_inmatch = val; }
public match_set_iskniferound(bool:val) { match_knife_round = val; }
public match_set_issechalf(bool:val) { match_issechalf = val; }
public match_set_isot(bool:val) { match_isovertime = val; }

public match_start(match_type)
{
	switch(match_type)
	{
		case 1:
		{
			match_set_inmatch(true);
			match_set_iskniferound(true);
			match_set_issechalf(false);
			match_set_isot(false);
			match_r3(match_type);
		}
		case 2:
		{
			match_set_inmatch(true);
			match_set_iskniferound(false);
			match_set_issechalf(false);
			match_set_isot(true);
			match_r3(match_type);
		}
		case 3:
		{
			match_set_inmatch(true);
			match_set_iskniferound(false);
			match_set_issechalf(true);
			match_set_isot(true);
			match_r3(match_type);
		}
		case 4:
		{
			match_set_inmatch(true);
			match_set_iskniferound(true);
			match_set_issechalf(false);
			match_set_isot(false);
			match_r3(match_type);
		}
		default:
		{
			match_set_inmatch(true);
			match_set_iskniferound(false);
			match_set_issechalf(false);
			match_set_isot(false);
			match_r3(match_type);
		}
	}
}


public match_r3(match_type)
{
	
}