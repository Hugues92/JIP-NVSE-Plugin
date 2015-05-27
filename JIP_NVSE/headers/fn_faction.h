#pragma once

DEFINE_COMMAND_PLUGIN(GetFactionReputationType, "None", 0, 1, kParams_JIP_OneFaction);
DEFINE_COMMAND_PLUGIN(SetFactionReputationType, "None", 0, 2, kParams_JIP_OneFaction_OneOptionalReputation);

bool Cmd_GetFactionReputationType_Execute(COMMAND_ARGS)
{
	UInt32 *refResult = (UInt32*)result;
	*refResult = 0;
	TESFaction *faction = NULL;
	if (ExtractArgs(EXTRACT_ARGS, &faction) && faction->reputation) *refResult = faction->reputation->refID;
	return true;
}

bool Cmd_SetFactionReputationType_Execute(COMMAND_ARGS)
{
	UInt32 *refResult = (UInt32*)result;
	*refResult = 0;
	TESFaction *faction = NULL;
	TESReputation *reputation = NULL;
	if (ExtractArgs(EXTRACT_ARGS, &faction, &reputation))
	{
		if (faction->reputation) *refResult = faction->reputation->refID;
		faction->reputation = reputation;
	}
	return true;
}
