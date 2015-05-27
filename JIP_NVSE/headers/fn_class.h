#pragma once

DEFINE_COMMAND_PLUGIN(GetClassTraitNumeric, "None", 0, 2, kParams_JIP_OneClass_OneInt);
DEFINE_COMMAND_PLUGIN(SetClassTraitNumeric, "None", 0, 3, kParams_JIP_OneClass_TwoInts);
DEFINE_COMMAND_PLUGIN(GetClassFlag, "None", 0, 2, kParams_JIP_OneClass_OneInt);
DEFINE_COMMAND_PLUGIN(SetClassFlag, "None", 0, 3, kParams_JIP_OneClass_TwoInts);

bool Cmd_GetClassTraitNumeric_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESClass *pClass = NULL;
	UInt32 traitID = 0;

	if (ExtractArgs(EXTRACT_ARGS, &pClass, &traitID) && ValidInput(traitID, 10))
	{
		if (traitID < 4) *result = pClass->tagSkills[traitID];
		else *result = pClass->attributes.attributes[traitID - 4];
	}
	return true;
}

bool Cmd_SetClassTraitNumeric_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESClass *pClass = NULL;
	UInt32 traitID = 0, val = 0;

	if (ExtractArgs(EXTRACT_ARGS, &pClass, &traitID, &val) && ValidInput(traitID, 10))
	{
		if (traitID < 4)
		{
			if ((val > 31) && (val < 46) && (val != 33))
			{
				*result = pClass->tagSkills[traitID];
				pClass->tagSkills[traitID] = val;
			}
		}
		else if ((val > 0) && (val < 11))
		{
			*result = pClass->attributes.attributes[traitID - 4];
			pClass->attributes.attributes[traitID - 4] = val;
		}
	}
	return true;
}

bool Cmd_GetClassFlag_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESClass *pClass = NULL;
	UInt32 flag = 0;

	if (ExtractArgs(EXTRACT_ARGS, &pClass, &flag) && ValidInput(flag, 19))
	{
		if (flag < 2) *result = (pClass->classFlags & (1 << flag)) ? 1 : 0;
		else *result = (pClass->services & (1 << (flag - 2))) ? 1 : 0;
	}
	return true;
}

bool Cmd_SetClassFlag_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESClass *pClass = NULL;
	UInt32 flag = 0, val = 0;

	if (ExtractArgs(EXTRACT_ARGS, &pClass, &flag, &val) && ValidInput(flag, 19))
	{
		if (flag < 2)
		{
			*result = (pClass->classFlags & (1 << flag)) ? 1 : 0;
			val ? (pClass->classFlags |= (1 << flag)) : (pClass->classFlags &= ~(1 << flag));
		}
		else
		{
			*result = (pClass->services & (1 << (flag - 2))) ? 1 : 0;
			val ? (pClass->services |= (1 << (flag - 2))) : (pClass->services &= ~(1 << (flag - 2)));
		}
	}
	return true;
}
