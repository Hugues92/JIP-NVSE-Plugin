#pragma once

DEFINE_COMMAND_PLUGIN(GetWeatherImageSpaceMod, "None", 0, 2, kParams_JIP_OneWeatherID_OneInt);
DEFINE_COMMAND_PLUGIN(SetWeatherImageSpaceMod, "None", 0, 3, kParams_JIP_OneWeatherID_OneInt_OneOptionalImageSpaceMod);
DEFINE_COMMAND_PLUGIN(GetWeatherTexture, "None", 0, 2, kParams_JIP_OneWeatherID_OneInt);
DEFINE_COMMAND_PLUGIN(SetWeatherTexture, "None", 0, 3, kParams_JIP_OneWeatherID_OneInt_OneString);
DEFINE_COMMAND_PLUGIN(GetWeatherPrecipitationModel, "None", 0, 2, kParams_JIP_OneWeatherID);
DEFINE_COMMAND_PLUGIN(SetWeatherPrecipitationModel, "None", 0, 3, kParams_JIP_OneWeatherID_OneString);
DEFINE_COMMAND_PLUGIN(GetWeatherTraitNumeric, "None", 0, 2, kParams_JIP_OneWeatherID_OneInt);
DEFINE_COMMAND_PLUGIN(SetWeatherTraitNumeric, "None", 0, 3, kParams_JIP_OneWeatherID_OneInt_OneFloat);
DEFINE_COMMAND_PLUGIN(GetWeatherRGBColor, "None", 0, 3, kParams_JIP_OneWeatherID_TwoInts);
DEFINE_COMMAND_PLUGIN(SetWeatherRGBColor, "None", 0, 4, kParams_JIP_OneWeatherID_ThreeInts);
DEFINE_COMMAND_PLUGIN(GetCurrentWeather, "None", 0, 0, NULL);

bool Cmd_GetWeatherImageSpaceMod_Execute(COMMAND_ARGS)
{
	UInt32 *refResult = (UInt32*)result, time = 0;
	*refResult = 0;
	TESWeather *weather = NULL;

	if (ExtractArgs(EXTRACT_ARGS, &weather, &time) && ValidInput(time, 5))
	{
		if (weather->imageSpaceMods[time]) *refResult = weather->imageSpaceMods[time]->refID;
	}
	return true;
}

bool Cmd_SetWeatherImageSpaceMod_Execute(COMMAND_ARGS)
{
	UInt32 *refResult = (UInt32*)result, time = 0;
	*refResult = 0;
	TESWeather *weather = NULL;
	TESImageSpaceModifier *imgSpcMod = NULL;

	if (ExtractArgs(EXTRACT_ARGS, &weather, &time, &imgSpcMod) && ValidInput(time, 5))
	{
		if (weather->imageSpaceMods[time]) *refResult = weather->imageSpaceMods[time]->refID;
		weather->imageSpaceMods[time] = imgSpcMod;
	}
	return true;
}

bool Cmd_GetWeatherTexture_Execute(COMMAND_ARGS)
{
	TESWeather *weather = NULL;
	UInt32 layer = 0;
	
	if (ExtractArgs(EXTRACT_ARGS, &weather, &layer) && ValidInput(layer, 3))
		g_string->Assign(PASS_COMMAND_ARGS, weather->layerTextures[layer].ddsPath.CStr());
	else g_string->Assign(PASS_COMMAND_ARGS, "");

	return true;
}

bool Cmd_SetWeatherTexture_Execute(COMMAND_ARGS)
{
	TESWeather *weather = NULL;
	UInt32 layer = 0;
	char ddsPath[512] = {0};
	
	if (ExtractArgs(EXTRACT_ARGS, &weather, &layer, &ddsPath) && ValidInput(layer, 3))
		weather->layerTextures[layer].ddsPath.Set(ddsPath);

	return true;
}

bool Cmd_GetWeatherPrecipitationModel_Execute(COMMAND_ARGS)
{
	TESWeather *weather = NULL;
	
	if (ExtractArgs(EXTRACT_ARGS, &weather))
		g_string->Assign(PASS_COMMAND_ARGS, weather->model.GetModelPath());
	else g_string->Assign(PASS_COMMAND_ARGS, "");

	return true;
}

bool Cmd_SetWeatherPrecipitationModel_Execute(COMMAND_ARGS)
{
	TESWeather *weather = NULL;
	char nifPath[512] = {0};
	
	if (ExtractArgs(EXTRACT_ARGS, &weather, &nifPath))
		weather->model.SetModelPath(nifPath);

	return true;
}

bool Cmd_GetWeatherTraitNumeric_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESWeather *weather = NULL;
	UInt32 traitID = 0;
	
	if (ExtractArgs(EXTRACT_ARGS, &weather, &traitID) && ValidInput(traitID, 20))
	{
		if (traitID < 4) *result = weather->cloudSpeed[traitID] / 2550.0;
		else if (traitID == 4) *result = weather->windSpeed / 255.0;
		else if (traitID == 5) *result = weather->transDelta / 1000.0;
		else if (traitID == 6) *result = weather->sunGlare / 255.0;
		else if (traitID == 7) *result = weather->sunDamage / 255.0;
		else if (traitID == 8) *result = weather->precipitationBeginFadeIn / 255.0;
		else if (traitID == 9) *result = weather->precipitationEndFadeOut / 255.0;
		else if (traitID == 10) *result = weather->lightningBeginFadeIn / 255.0;
		else if (traitID == 11) *result = weather->lightningEndFadeOut / 255.0;
		else if (traitID == 12) *result = weather->lightningFrequency / 255.0;
		else if (traitID == 13) *result = weather->weatherClassification;
		else if (traitID == 14) *result = RGBHexToDec(weather->lightningColor);
		else *result = weather->fogDistance[traitID - 15];
	}
	return true;
}

bool Cmd_SetWeatherTraitNumeric_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESWeather *weather = NULL;
	UInt32 traitID = 0;
	float val = 0;
	
	if (ExtractArgs(EXTRACT_ARGS, &weather, &traitID, &val) && ValidInput(traitID, 20))
	{
		if (traitID < 4)
		{
			*result = weather->cloudSpeed[traitID] / 2550.0;
			if (val > 0.1) weather->cloudSpeed[traitID] = 255;
			else if (val < 0) weather->cloudSpeed[traitID] = 0;
			else weather->cloudSpeed[traitID] = val * 2550;
		}
		else if (traitID == 5)
		{
			*result = weather->transDelta / 1000.0;
			if (val > 0.25) weather->transDelta = 255;
			else if (val < 0) weather->transDelta = 0;
			else weather->transDelta = val * 1000;
		}
		else if (traitID < 13)
		{
			if (val > 1) val = 255;
			else if (val < 0) val = 0;
			else val *= 255;

			if (traitID == 4)
			{
				*result = weather->windSpeed / 255.0;
				weather->windSpeed = val;
			}
			else if (traitID == 6)
			{
				*result = weather->sunGlare / 255.0;
				weather->sunGlare = val;
			}
			else if (traitID == 7)
			{
				*result = weather->sunDamage / 255.0;
				weather->sunDamage = val;
			}
			else if (traitID == 8)
			{
				*result = weather->precipitationBeginFadeIn / 255.0;
				weather->precipitationBeginFadeIn = val;
			}
			else if (traitID == 9)
			{
				*result = weather->precipitationEndFadeOut / 255.0;
				weather->precipitationEndFadeOut = val;
			}
			else if (traitID == 10)
			{
				*result = weather->lightningBeginFadeIn / 255.0;
				weather->lightningBeginFadeIn = val;
			}
			else if (traitID == 11)
			{
				*result = weather->lightningEndFadeOut / 255.0;
				weather->lightningEndFadeOut = val;
			}
			else
			{
				*result = weather->lightningFrequency / 255.0;
				weather->lightningFrequency = val;
			}
		}
		else if (traitID == 13)
		{
			if ((val == 0) || (val == 1) || (val == 2) || (val == 4) || (val == 8))
			{
				*result = weather->weatherClassification;
				weather->weatherClassification = val;
			}
		}
		else if (traitID > 14)
		{
			*result = weather->fogDistance[traitID - 15];
			weather->fogDistance[traitID - 15] = val;
		}
		else if ((val >= 0) && (val <= 255255255))
		{
			*result = RGBHexToDec(weather->lightningColor);
			weather->lightningColor = RGBDecToHex((UInt32)val);
		}
	}
	return true;
}

bool Cmd_GetWeatherRGBColor_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESWeather *weather = NULL;
	UInt32 type = 0, time = 0;
	
	if (ExtractArgs(EXTRACT_ARGS, &weather, &type, &time) && ValidInput(type, 9) && ValidInput(time, 5))
		*result = RGBHexToDec(weather->colors[type][time]);

	return true;
}

bool Cmd_SetWeatherRGBColor_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESWeather *weather = NULL;
	UInt32 type = 0, time = 0, rgb = 0;
	
	if (ExtractArgs(EXTRACT_ARGS, &weather, &type, &time, &rgb) && ValidInput(type, 9) && ValidInput(time, 5) && ValidInput(rgb, 255255255))
	{
		*result = RGBHexToDec(weather->colors[type][time]);
		weather->colors[type][time] = RGBDecToHex(rgb);
	}
	return true;
}

bool Cmd_GetCurrentWeather_Execute(COMMAND_ARGS)
{
	UInt32 *refResult = (UInt32*)result;
	*refResult = 0;

	Sky *sky = Sky::GetSingleton();
	if (sky && sky->firstWeather) *refResult = sky->firstWeather->refID;
	return true;
}
