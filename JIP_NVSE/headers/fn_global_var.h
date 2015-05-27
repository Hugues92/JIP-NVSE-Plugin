#pragma once

DEFINE_COMMAND_PLUGIN(GetGlobalVariable, "None", 0, 1, kParams_JIP_OneGlobal);
DEFINE_COMMAND_PLUGIN(SetGlobalVariable, "None", 0, 2, kParams_JIP_OneGlobal_OneFloat);

bool Cmd_GetGlobalVariable_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESGlobal *global = NULL;
	if (ExtractArgs(EXTRACT_ARGS, &global)) *result = global->data;
	return true;
}

bool Cmd_SetGlobalVariable_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESGlobal *global = NULL;
	float val = 0;

	if (ExtractArgs(EXTRACT_ARGS, &global, &val))
	{
		*result = global->data;
		global->data = val;
	}
	return true;
}
