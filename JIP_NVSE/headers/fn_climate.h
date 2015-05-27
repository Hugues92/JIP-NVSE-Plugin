#pragma once

DEFINE_COMMAND_PLUGIN(GetClimateSunTexture, "None", 0, 1, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(SetClimateSunTexture, "None", 0, 2, kParams_JIP_OneForm_OneString);
DEFINE_COMMAND_PLUGIN(GetClimateSunGlareTexture, "None", 0, 1, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(SetClimateSunGlareTexture, "None", 0, 2, kParams_JIP_OneForm_OneString);
DEFINE_COMMAND_PLUGIN(GetClimateNightSkyModel, "None", 0, 1, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(SetClimateNightSkyModel, "None", 0, 2, kParams_JIP_OneForm_OneString);
DEFINE_COMMAND_PLUGIN(GetClimateTraitNumeric, "None", 0, 2, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(SetClimateTraitNumeric, "None", 0, 3, kParams_JIP_OneForm_TwoInts);
DEFINE_COMMAND_PLUGIN(GetClimateNumWeatherTypes, "None", 0, 1, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(GetClimateNthWeatherType, "None", 0, 2, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(GetClimateNthWeatherChance, "None", 0, 2, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(GetClimateNthWeatherGlobal, "None", 0, 2, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(ClimateAddWeatherType, "None", 0, 4, kParams_JIP_OneForm_OneWeatherID_OneInt_OneOptionalGlobal);
DEFINE_COMMAND_PLUGIN(ClimateRemoveWeatherType, "None", 0, 2, kParams_JIP_OneForm_OneWeatherID);
DEFINE_COMMAND_PLUGIN(GetCurrentClimate, "None", 0, 0, NULL);
DEFINE_COMMAND_PLUGIN(SetCurrentClimate, "None", 0, 1, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(RefreshCurrentClimate, "None", 0, 0, NULL);
DEFINE_COMMAND_PLUGIN(GetWorldspaceClimate, "None", 0, 1, kParams_JIP_OneWorldspace);
DEFINE_COMMAND_PLUGIN(SetWorldspaceClimate, "None", 0, 2, kParams_JIP_OneWorldspace_OneForm);

bool Cmd_GetClimateSunTexture_Execute(COMMAND_ARGS)
{
	TESForm *form = NULL;
	
	if (ExtractArgs(EXTRACT_ARGS, &form))
	{
		TESClimate *climate = DYNAMIC_CAST(form, TESForm, TESClimate);
		if (climate)
		{
			g_string->Assign(PASS_COMMAND_ARGS, climate->sunTexture.ddsPath.CStr());
			return true;
		}
	}
	g_string->Assign(PASS_COMMAND_ARGS, "");

	return true;
}

bool Cmd_SetClimateSunTexture_Execute(COMMAND_ARGS)
{
	TESForm *form = NULL;
	char ddsPath[512] = {0};
	
	if (ExtractArgs(EXTRACT_ARGS, &form, &ddsPath))
	{
		TESClimate *climate = DYNAMIC_CAST(form, TESForm, TESClimate);
		if (climate) climate->sunTexture.ddsPath.Set(ddsPath);
	}
	return true;
}

bool Cmd_GetClimateSunGlareTexture_Execute(COMMAND_ARGS)
{
	TESForm *form = NULL;
	
	if (ExtractArgs(EXTRACT_ARGS, &form))
	{
		TESClimate *climate = DYNAMIC_CAST(form, TESForm, TESClimate);
		if (climate)
		{
			g_string->Assign(PASS_COMMAND_ARGS, climate->sunGlareTexture.ddsPath.CStr());
			return true;
		}
	}
	g_string->Assign(PASS_COMMAND_ARGS, "");

	return true;
}

bool Cmd_SetClimateSunGlareTexture_Execute(COMMAND_ARGS)
{
	TESForm *form = NULL;
	char ddsPath[512] = {0};
	
	if (ExtractArgs(EXTRACT_ARGS, &form, &ddsPath))
	{
		TESClimate *climate = DYNAMIC_CAST(form, TESForm, TESClimate);
		if (climate) climate->sunGlareTexture.ddsPath.Set(ddsPath);
	}
	return true;
}

bool Cmd_GetClimateNightSkyModel_Execute(COMMAND_ARGS)
{
	TESForm *form = NULL;
	
	if (ExtractArgs(EXTRACT_ARGS, &form))
	{
		TESClimate *climate = DYNAMIC_CAST(form, TESForm, TESClimate);
		if (climate)
		{
			g_string->Assign(PASS_COMMAND_ARGS, climate->nightSkyModel.GetModelPath());
			return true;
		}
	}
	g_string->Assign(PASS_COMMAND_ARGS, "");

	return true;
}

bool Cmd_SetClimateNightSkyModel_Execute(COMMAND_ARGS)
{
	TESForm *form = NULL;
	char nifPath[512] = {0};
	
	if (ExtractArgs(EXTRACT_ARGS, &form, &nifPath))
	{
		TESClimate *climate = DYNAMIC_CAST(form, TESForm, TESClimate);
		if (climate) climate->nightSkyModel.SetModelPath(nifPath);
	}
	return true;
}

bool Cmd_GetClimateTraitNumeric_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESForm *form = NULL;
	UInt32 traitID = 0;
	
	if (ExtractArgs(EXTRACT_ARGS, &form, &traitID) && ValidInput(traitID, 6))
	{
		TESClimate *climate = DYNAMIC_CAST(form, TESForm, TESClimate);
		if (climate)
		{
			if (!traitID) *result = climate->sunriseBegin * 10;
			else if (traitID == 1) *result = climate->sunriseEnd * 10;
			else if (traitID == 2) *result = climate->sunsetBegin * 10;
			else if (traitID == 3) *result = climate->sunsetEnd * 10;
			else if (traitID == 4) *result = climate->volatility;
			else if (traitID == 5) *result = climate->phaseLength >> 6;
			else *result = climate->phaseLength & 63;
		}
	}
	return true;
}

bool Cmd_SetClimateTraitNumeric_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESForm *form = NULL;
	UInt32 traitID = 0, val = 0;
	
	if (ExtractArgs(EXTRACT_ARGS, &form, &traitID, &val) && ValidInput(traitID, 6))
	{
		TESClimate *climate = DYNAMIC_CAST(form, TESForm, TESClimate);
		if (climate)
		{
			if (val < 0) val = 0;
			if (traitID < 4)
			{
				if (val > 1430) val = 143;
				else val /= 10;

				if (!traitID)
				{
					*result = climate->sunriseBegin * 10;
					climate->sunriseBegin = val;
				}
				else if (traitID == 1)
				{
					*result = climate->sunriseEnd * 10;
					climate->sunriseEnd = val;
				}
				else if (traitID == 2)
				{
					*result = climate->sunsetBegin * 10;
					climate->sunsetBegin = val;
				}
				else if (traitID == 3)
				{
					*result = climate->sunsetEnd * 10;
					climate->sunsetEnd = val;
				}
			}
			else if (traitID == 4)
			{
				*result = climate->volatility;
				if (val > 255) val = 255;
				climate->volatility = val;
			}
			else if (traitID == 5)
			{
				*result = climate->phaseLength >> 6;
				if (val > 3) val = 192;
				else val <<= 6;
				climate->phaseLength = (climate->phaseLength & 63) + val;
			}
			else
			{
				*result = climate->phaseLength & 63;
				if (val > 63) val = 63;
				climate->phaseLength = (climate->phaseLength & 192) + val;
			}
		}
	}
	return true;
}

bool Cmd_GetClimateNumWeatherTypes_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESForm *form = NULL;
	
	if (ExtractArgs(EXTRACT_ARGS, &form))
	{
		TESClimate *climate = DYNAMIC_CAST(form, TESForm, TESClimate);
		if (climate) *result = climate->weatherTypes.Count();
	}
	return true;
}

bool Cmd_GetClimateNthWeatherType_Execute(COMMAND_ARGS)
{
	UInt32 *refResult = (UInt32*)result, idx = 0;
	*refResult = 0;
	TESForm *form = NULL;

	if (ExtractArgs(EXTRACT_ARGS, &form, &idx))
	{
		TESClimate *climate = DYNAMIC_CAST(form, TESForm, TESClimate);
		if (climate && (idx < climate->weatherTypes.Count()))
		{
			TESWeather *weather = climate->weatherTypes.GetNthItem(idx)->weather;
			if (weather) *refResult = weather->refID;
		}
	}
	return true;
}

bool Cmd_GetClimateNthWeatherChance_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESForm *form = NULL;
	UInt32 idx = 0;

	if (ExtractArgs(EXTRACT_ARGS, &form, &idx))
	{
		TESClimate *climate = DYNAMIC_CAST(form, TESForm, TESClimate);
		if (climate && (idx < climate->weatherTypes.Count()))
			*result = climate->weatherTypes.GetNthItem(idx)->chance;
	}
	return true;
}

bool Cmd_GetClimateNthWeatherGlobal_Execute(COMMAND_ARGS)
{
	UInt32 *refResult = (UInt32*)result, idx = 0;
	*refResult = 0;
	TESForm *form = NULL;

	if (ExtractArgs(EXTRACT_ARGS, &form, &idx))
	{
		TESClimate *climate = DYNAMIC_CAST(form, TESForm, TESClimate);
		if (climate && (idx < climate->weatherTypes.Count()))
		{
			TESGlobal *global = climate->weatherTypes.GetNthItem(idx)->global;
			if (global) *refResult = global->refID;
		}
	}
	return true;
}

WeatherEntry *GetWeatherEntry(TESClimate *climate, TESWeather *weather, bool rmv = false)
{
	WeatherEntry *wType;
	for (SInt32 idx = climate->weatherTypes.Count() - 1; idx >= 0; idx--)
	{
		wType = climate->weatherTypes.GetNthItem(idx);
		if (wType->weather == weather)
		{
			if (rmv) climate->weatherTypes.RemoveNth(idx);
			return wType;
		}
	}
	if (rmv) return NULL;
	wType = (WeatherEntry*)FormHeap_Allocate(sizeof(WeatherEntry));
	climate->weatherTypes.AddAt(wType, eListEnd);
	return wType;
}

bool Cmd_ClimateAddWeatherType_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESForm *form = NULL;
	TESWeather *weather = NULL;
	UInt32 chance = 0;
	TESGlobal *global = NULL;

	if (ExtractArgs(EXTRACT_ARGS, &form, &weather, &chance, &global))
	{
		TESClimate *climate = DYNAMIC_CAST(form, TESForm, TESClimate);
		if (climate)
		{
			WeatherEntry *wType = GetWeatherEntry(climate, weather);
			wType->Set(weather, chance % 101, global);
			*result = 1;
		}
	}
	return true;
}

bool Cmd_ClimateRemoveWeatherType_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESForm *form = NULL;
	TESWeather *weather = NULL;

	if (ExtractArgs(EXTRACT_ARGS, &form, &weather))
	{
		TESClimate *climate = DYNAMIC_CAST(form, TESForm, TESClimate);
		if (climate) return GetWeatherEntry(climate, weather, true) ? 1 : 0;
	}
	return true;
}

bool Cmd_GetCurrentClimate_Execute(COMMAND_ARGS)
{
	UInt32 *refResult = (UInt32*)result;
	*refResult = 0;

	Sky *sky = Sky::GetSingleton();
	if (sky && sky->firstClimate) *refResult = sky->firstClimate->refID;
	return true;
}

bool Cmd_SetCurrentClimate_Execute(COMMAND_ARGS)
{
	UInt32 *refResult = (UInt32*)result;
	*refResult = 0;
	TESForm *form = NULL;

	if (ExtractArgs(EXTRACT_ARGS, &form))
	{
		TESClimate *climate = DYNAMIC_CAST(form, TESForm, TESClimate);
		if (climate)
		{
			Sky *sky = Sky::GetSingleton();
			if (sky)
			{
				if (sky->firstClimate) *refResult = sky->firstClimate->refID;
				sky->firstClimate = climate;
			}
		}
	}
	return true;
}

bool Cmd_RefreshCurrentClimate_Execute(COMMAND_ARGS)
{
	Sky *sky = Sky::GetSingleton();
	if (sky) sky->RefreshClimate(sky->firstClimate);
	return true;
}

bool Cmd_GetWorldspaceClimate_Execute(COMMAND_ARGS)
{
	UInt32 *refResult = (UInt32*)result;
	*refResult = 0;
	TESWorldSpace *wldSpc = NULL;

	if (ExtractArgs(EXTRACT_ARGS, &wldSpc))
	{
		while (wldSpc->parent && (wldSpc->parentFlags & 16)) wldSpc = wldSpc->parent;
		if (wldSpc->climate) *refResult = wldSpc->climate->refID;
	}
	return true;
}

bool Cmd_SetWorldspaceClimate_Execute(COMMAND_ARGS)
{
	UInt32 *refResult = (UInt32*)result;
	*refResult = 0;
	TESWorldSpace *wldSpc = NULL;
	TESForm *clmFrm = NULL;

	if (ExtractArgs(EXTRACT_ARGS, &wldSpc, &clmFrm))
	{
		TESClimate *climate = DYNAMIC_CAST(clmFrm, TESForm, TESClimate);
		if (climate)
		{
			if (wldSpc->climate) *refResult = wldSpc->climate->refID;
			if (wldSpc->parent && (wldSpc->parentFlags & 16)) wldSpc->parentFlags -= 16;
			wldSpc->climate = climate;
			PlayerCharacter *pc = PlayerCharacter::GetSingleton();
			if (pc && pc->parentCell)
			{
				TESWorldSpace *pcWspc = pc->parentCell->worldSpace;
				if (pcWspc)
				{
					while (pcWspc->parent && (pcWspc->parentFlags & 16)) pcWspc = pcWspc->parent;
					if (pcWspc == wldSpc)
					{
						Sky *sky = Sky::GetSingleton();
						if (sky) sky->firstClimate = climate;
					}
				}
			}
		}
	}
	return true;
}
