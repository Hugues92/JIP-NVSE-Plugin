#pragma once

DEFINE_COMMAND_PLUGIN(GetLightTraitNumeric, "None", 0, 2, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(SetLightTraitNumeric, "None", 0, 3, kParams_JIP_OneForm_OneInt_OneFloat);
DEFINE_COMMAND_PLUGIN(GetLightFlag, "None", 0, 2, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(SetLightFlag, "None", 0, 3, kParams_JIP_OneForm_TwoInts);

bool Cmd_GetLightTraitNumeric_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESForm *form = NULL;
	UInt32 traitID = 0;

	if (ExtractArgs(EXTRACT_ARGS, &form, &traitID) && ValidInput(traitID, 6))
	{
		TESObjectLIGH *light = DYNAMIC_CAST(form, TESForm, TESObjectLIGH);
		if (light)
		{
			if (!traitID) *result = light->radius;
			else if (traitID == 1) *result = light->red;
			else if (traitID == 2) *result = light->green;
			else if (traitID == 3) *result = light->blue;
			else if (traitID == 4) *result = light->falloffExp;
			else if (traitID == 5) *result = light->FOV;
			else *result = light->fadeValue;
		}
	}
	return true;
}

bool Cmd_SetLightTraitNumeric_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESForm *form = NULL;
	UInt32 traitID = 0;
	float val = 0;

	if (ExtractArgs(EXTRACT_ARGS, &form, &traitID, &val) && ValidInput(traitID, 6))
	{
		TESObjectLIGH *light = DYNAMIC_CAST(form, TESForm, TESObjectLIGH);
		if (light)
		{
			if (!traitID)
			{
				*result = light->radius;
				light->radius = val;
			}
			else if (traitID == 1)
			{
				*result = light->red;
				light->red = val;
			}
			else if (traitID == 2)
			{
				*result = light->green;
				light->green = val;
			}
			else if (traitID == 3)
			{
				*result = light->blue;
				light->blue = val;
			}
			else if (traitID == 4)
			{
				*result = light->falloffExp;
				light->falloffExp = val;
			}
			else if (traitID == 5)
			{
				*result = light->FOV;
				light->FOV = val;
			}
			else
			{
				*result = light->fadeValue;
				light->fadeValue = val;
			}
		}
	}
	return true;
}

bool Cmd_GetLightFlag_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESForm *form = NULL;
	UInt32 flag = 0;

	if (ExtractArgs(EXTRACT_ARGS, &form, &flag) && ValidInput(flag, 10))
	{
		TESObjectLIGH *light = DYNAMIC_CAST(form, TESForm, TESObjectLIGH);
		if (light) *result = (light->lightFlags & (1 << flag)) ? 1 : 0;
	}
	return true;
}

bool Cmd_SetLightFlag_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESForm *form = NULL;
	UInt32 flag = 0, val = 0;

	if (ExtractArgs(EXTRACT_ARGS, &form, &flag, &val) && ValidInput(flag, 10))
	{
		TESObjectLIGH *light = DYNAMIC_CAST(form, TESForm, TESObjectLIGH);
		if (light)
		{
			*result = (light->lightFlags & (1 << flag)) ? 1 : 0;
			light->SetFlag(1 << flag, val != 0);
		}
	}
	return true;
}
