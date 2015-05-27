#pragma once

DEFINE_COMMAND_PLUGIN(GetSoundTraitNumeric, "None", 0, 2, kParams_JIP_OneSound_OneInt);
DEFINE_COMMAND_PLUGIN(SetSoundTraitNumeric, "None", 0, 3, kParams_JIP_OneSound_OneInt_OneFloat);
DEFINE_COMMAND_PLUGIN(GetSoundFlag, "None", 0, 2, kParams_JIP_OneSound_OneInt);
DEFINE_COMMAND_PLUGIN(SetSoundFlag, "None", 0, 3, kParams_JIP_OneSound_TwoInts);
DEFINE_COMMAND_PLUGIN(GetSoundSourceFile, "None", 0, 1, kParams_JIP_OneSound);
DEFINE_COMMAND_PLUGIN(SetSoundSourceFile, "None", 0, 2, kParams_JIP_OneSound_OneString);

bool Cmd_GetSoundTraitNumeric_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESSound *sound = NULL;
	UInt32 traitID = 0;

	if (ExtractArgs(EXTRACT_ARGS, &sound, &traitID) && ValidInput(traitID, 12))
	{
		if (!traitID) *result = sound->minAttenuationDist * 5;
		else if (traitID == 1) *result = sound->maxAttenuationDist * 100;
		else if (traitID == 2) *result = sound->frequencyAdj;
		else if (traitID == 3) *result = sound->staticAttenuation / -100;
		else if (traitID == 4) *result = sound->endsAt * 5.62353;
		else if (traitID == 5) *result = sound->startsAt * 5.62353;
		else if (traitID < 11) *result = sound->attenuationCurve[traitID - 6];
		else if (traitID == 11) *result = sound->reverbAttenuation;
		else *result = sound->priority;
	}
	return true;
}

bool Cmd_SetSoundTraitNumeric_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESSound *sound = NULL;
	UInt32 traitID = 0;
	float val = 0;

	if (ExtractArgs(EXTRACT_ARGS, &sound, &traitID, &val) && ValidInput(traitID, 12))
	{
		if (!traitID)
		{
			if (val < 0) val = 0;
			else if (val > 1275) val = 255;
			else val /= 5;

			*result = sound->minAttenuationDist * 5;
			sound->minAttenuationDist = val;
		}
		else if (traitID == 1)
		{
			if (val < 0) val = 0;
			else if (val > 25500) val = 255;
			else val /= 100;

			*result = sound->maxAttenuationDist * 100;
			sound->maxAttenuationDist = val;
		}
		else if (traitID == 2)
		{
			if (val < -100) val = -100;
			else if (val > 100) val = 100;

			*result = sound->frequencyAdj;
			sound->frequencyAdj = val;
		}
		else if (traitID == 3)
		{
			val = abs(val);
			if (val > 100) val = 10000;
			else val *= 100;

			*result = sound->staticAttenuation / -100;
			sound->staticAttenuation = val;
		}
		else if (traitID < 6)
		{
			if (val < 0) val = 0;
			else if (val > 1434) val = 255;
			else val /= 5.62353;

			if (traitID == 4)
			{
				*result = sound->endsAt * 5.62353;
				sound->endsAt = val;
			}
			else
			{
				*result = sound->startsAt * 5.62353;
				sound->startsAt = val;
			}
		}
		else if (traitID < 11)
		{
			if (val < 0) val = 0;
			else if (val > 100) val = 100;

			*result = sound->attenuationCurve[traitID - 6];
			sound->attenuationCurve[traitID - 6] = val;
		}
		else if (traitID == 11)
		{
			if (val < 0) val = 0;
			else if (val > 100) val = 100;

			*result = sound->reverbAttenuation;
			sound->reverbAttenuation = val;
		}
		else
		{
			if (val < 0) val = 0;
			else if (val > 255) val = 255;

			*result = sound->priority;
			sound->priority = val;
		}
	}
	return true;
}

bool Cmd_GetSoundFlag_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESSound *sound = NULL;
	UInt32 flag = 0;

	if (ExtractArgs(EXTRACT_ARGS, &sound, &flag) && ValidInput(flag, 13))
		*result = (sound->soundFlags & (1 << flag)) ? 1 : 0;

	return true;
}

bool Cmd_SetSoundFlag_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESSound *sound = NULL;
	UInt32 flag = 0, val = 0;

	if (ExtractArgs(EXTRACT_ARGS, &sound, &flag, &val) && ValidInput(flag, 13))
	{
		*result = (sound->soundFlags & (1 << flag)) ? 1 : 0;
		sound->SetFlag(1 << flag, val != 0);
	}
	return true;
}

bool Cmd_GetSoundSourceFile_Execute(COMMAND_ARGS)
{
	TESSound *sound = NULL;
	if (ExtractArgs(EXTRACT_ARGS, &sound)) g_string->Assign(PASS_COMMAND_ARGS, sound->soundFile.path.CStr());
	else g_string->Assign(PASS_COMMAND_ARGS, "");
	return true;
}

bool Cmd_SetSoundSourceFile_Execute(COMMAND_ARGS)
{
	TESSound *sound = NULL;
	char filePath[512] = {0};
	if (ExtractArgs(EXTRACT_ARGS, &sound, &filePath)) sound->soundFile.Set(filePath);
	return true;
}
