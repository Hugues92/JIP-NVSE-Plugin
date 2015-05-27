#pragma once

DEFINE_COMMAND_PLUGIN(GetPackageFlag, "None", 0, 2, kParams_JIP_OneAIPackage_OneInt);
DEFINE_COMMAND_PLUGIN(SetPackageFlag, "None", 0, 3, kParams_JIP_OneAIPackage_TwoInts);

bool Cmd_GetPackageFlag_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESPackage *package = NULL;
	UInt32 flag = 0;
	if (ExtractArgs(EXTRACT_ARGS, &package, &flag) && ValidInput(flag, 31)) *result = package->IsFlagSet(1 << flag);
	return true;
}

bool Cmd_SetPackageFlag_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESPackage *package = NULL;
	UInt32 flag = 0, val = 0;
	if (ExtractArgs(EXTRACT_ARGS, &package, &flag, &val) && ValidInput(flag, 31))
	{
		*result = package->IsFlagSet(1 << flag);
		package->SetFlag(1 << flag, val != 0);
	}
	return true;
}
