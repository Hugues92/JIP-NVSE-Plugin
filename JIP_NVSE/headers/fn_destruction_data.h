#pragma once

DEFINE_COMMAND_PLUGIN(GetDestructionDataHealth, "None", 0, 1, kParams_OneObject);
DEFINE_COMMAND_PLUGIN(SetDestructionDataHealth, "None", 0, 2, kParams_JIP_OneObject_OneInt);
DEFINE_COMMAND_PLUGIN(GetDestructionDataTargetable, "None", 0, 1, kParams_OneObject);
DEFINE_COMMAND_PLUGIN(SetDestructionDataTargetable, "None", 0, 2, kParams_JIP_OneObject_OneInt);
DEFINE_COMMAND_PLUGIN(GetDestructionDataNumStages, "None", 0, 1, kParams_OneObject);
DEFINE_COMMAND_PLUGIN(GetNthDestructionStageTrait, "None", 0, 3, kParams_JIP_OneObject_TwoInts);
DEFINE_COMMAND_PLUGIN(SetNthDestructionStageTrait, "None", 0, 4, kParams_JIP_OneObject_ThreeInts);
DEFINE_COMMAND_PLUGIN(GetNthDestructionStageExplosion, "None", 0, 2, kParams_JIP_OneObject_OneInt);
DEFINE_COMMAND_PLUGIN(SetNthDestructionStageExplosion, "None", 0, 3, kParams_JIP_OneObject_OneInt_OneOptionalForm);
DEFINE_COMMAND_PLUGIN(GetNthDestructionStageDebris, "None", 0, 2, kParams_JIP_OneObject_OneInt);
DEFINE_COMMAND_PLUGIN(SetNthDestructionStageDebris, "None", 0, 3, kParams_JIP_OneObject_OneInt_OneOptionalForm);
DEFINE_COMMAND_PLUGIN(GetNthDestructionStageReplacement, "None", 0, 2, kParams_JIP_OneObject_OneInt);
DEFINE_COMMAND_PLUGIN(SetNthDestructionStageReplacement, "None", 0, 3, kParams_JIP_OneObject_OneInt_OneString);

bool Cmd_GetDestructionDataHealth_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESForm *object = NULL;

	if (ExtractArgs(EXTRACT_ARGS, &object))
	{
		BGSDestructibleObjectForm *destr = DYNAMIC_CAST(object, TESForm, BGSDestructibleObjectForm);
		if (destr && destr->data) *result = destr->data->health;
	}
	return true;
}

bool Cmd_SetDestructionDataHealth_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESForm *object = NULL;
	UInt32 value = 0;

	if (ExtractArgs(EXTRACT_ARGS, &object, &value))
	{
		BGSDestructibleObjectForm *destr = DYNAMIC_CAST(object, TESForm, BGSDestructibleObjectForm);
		if (destr && destr->data)
		{
			*result = destr->data->health;
			destr->data->health = value;
		}
	}
	return true;
}

bool Cmd_GetDestructionDataTargetable_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESForm *object = NULL;

	if (ExtractArgs(EXTRACT_ARGS, &object))
	{
		BGSDestructibleObjectForm *destr = DYNAMIC_CAST(object, TESForm, BGSDestructibleObjectForm);
		if (destr && destr->data) *result = destr->data->targetable ? 1 : 0;
	}
	return true;
}

bool Cmd_SetDestructionDataTargetable_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESForm *object = NULL;
	UInt32 value = 0;

	if (ExtractArgs(EXTRACT_ARGS, &object, &value))
	{
		BGSDestructibleObjectForm *destr = DYNAMIC_CAST(object, TESForm, BGSDestructibleObjectForm);
		if (destr && destr->data)
		{
			*result = destr->data->targetable ? 1 : 0;
			destr->data->targetable = value ? true : false;
		}
	}
	return true;
}

bool Cmd_GetDestructionDataNumStages_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESForm *object = NULL;

	if (ExtractArgs(EXTRACT_ARGS, &object))
	{
		BGSDestructibleObjectForm *destr = DYNAMIC_CAST(object, TESForm, BGSDestructibleObjectForm);
		if (destr && destr->data) *result = destr->data->stageCount;
	}
	return true;
}

bool Cmd_GetNthDestructionStageTrait_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESForm *object = NULL;
	UInt32 idx = 0, traitID = 0;

	if (ExtractArgs(EXTRACT_ARGS, &object, &idx, &traitID) && ValidInput(traitID, 4))
	{
		BGSDestructibleObjectForm *destr = DYNAMIC_CAST(object, TESForm, BGSDestructibleObjectForm);
		if (destr && destr->data && (idx >= 0) && (idx < destr->data->stageCount))
		{
			if (!traitID) *result = destr->data->stages[idx]->healthPrc;
			else if (traitID == 1) *result = destr->data->stages[idx]->selfDmgSec;
			else if (traitID == 2) *result = destr->data->stages[idx]->dmgStage;
			else if (traitID == 3) *result = destr->data->stages[idx]->flags;
			else *result = destr->data->stages[idx]->debrisCount;
		}
	}
	return true;
}

bool Cmd_SetNthDestructionStageTrait_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESForm *object = NULL;
	UInt32 idx = 0, traitID = 0, value = 0;

	if (ExtractArgs(EXTRACT_ARGS, &object, &idx, &traitID, &value) && ValidInput(traitID, 4))
	{
		BGSDestructibleObjectForm *destr = DYNAMIC_CAST(object, TESForm, BGSDestructibleObjectForm);
		if (destr && destr->data && (idx >= 0) && (idx < destr->data->stageCount))
		{
			if (value < 0) value = 0;
			if (!traitID)
			{
				*result = destr->data->stages[idx]->healthPrc;
				destr->data->stages[idx]->healthPrc = value % 101;
			}
			else if (traitID == 1)
			{
				*result = destr->data->stages[idx]->selfDmgSec;
				destr->data->stages[idx]->selfDmgSec = value;
			}
			else if (traitID == 2)
			{
				*result = destr->data->stages[idx]->dmgStage;
				destr->data->stages[idx]->dmgStage = value % 9;
			}
			else if (traitID == 3)
			{
				*result = destr->data->stages[idx]->flags;
				destr->data->stages[idx]->flags = value & 7;
			}
			else
			{
				*result = destr->data->stages[idx]->debrisCount;
				destr->data->stages[idx]->debrisCount = value;
			}
		}
	}
	return true;
}

bool Cmd_GetNthDestructionStageExplosion_Execute(COMMAND_ARGS)
{
	UInt32 *refResult = (UInt32*)result, idx = 0;
	*refResult = 0;
	TESForm *object = NULL;

	if (ExtractArgs(EXTRACT_ARGS, &object, &idx))
	{
		BGSDestructibleObjectForm *destr = DYNAMIC_CAST(object, TESForm, BGSDestructibleObjectForm);
		if (destr && destr->data && (idx >= 0) && (idx < destr->data->stageCount) && destr->data->stages[idx]->explosion)
			*refResult = destr->data->stages[idx]->explosion->refID;
	}
	return true;
}

bool Cmd_SetNthDestructionStageExplosion_Execute(COMMAND_ARGS)
{
	UInt32 *refResult = (UInt32*)result, idx = 0;
	*refResult = 0;
	TESForm *object = NULL, *explForm = NULL;

	if (ExtractArgs(EXTRACT_ARGS, &object, &idx, &explForm))
	{
		BGSDestructibleObjectForm *destr = DYNAMIC_CAST(object, TESForm, BGSDestructibleObjectForm);
		if (destr && destr->data && (idx >= 0) && (idx < destr->data->stageCount))
		{
			if (destr->data->stages[idx]->explosion) *refResult = destr->data->stages[idx]->explosion->refID;
			destr->data->stages[idx]->explosion = explForm ? DYNAMIC_CAST(explForm, TESForm, BGSExplosion) : NULL;
		}
	}
	return true;
}

bool Cmd_GetNthDestructionStageDebris_Execute(COMMAND_ARGS)
{
	UInt32 *refResult = (UInt32*)result, idx = 0;
	*refResult = 0;
	TESForm *object = NULL;

	if (ExtractArgs(EXTRACT_ARGS, &object, &idx))
	{
		BGSDestructibleObjectForm *destr = DYNAMIC_CAST(object, TESForm, BGSDestructibleObjectForm);
		if (destr && destr->data && (idx >= 0) && (idx < destr->data->stageCount) && destr->data->stages[idx]->debris)
			*refResult = destr->data->stages[idx]->debris->refID;
	}
	return true;
}

bool Cmd_SetNthDestructionStageDebris_Execute(COMMAND_ARGS)
{
	UInt32 *refResult = (UInt32*)result, idx = 0;
	*refResult = 0;
	TESForm *object = NULL, *dbrForm = NULL;

	if (ExtractArgs(EXTRACT_ARGS, &object, &idx, &dbrForm))
	{
		BGSDestructibleObjectForm *destr = DYNAMIC_CAST(object, TESForm, BGSDestructibleObjectForm);
		if (destr && destr->data && (idx >= 0) && (idx < destr->data->stageCount))
		{
			if (destr->data->stages[idx]->debris) *refResult = destr->data->stages[idx]->debris->refID;
			destr->data->stages[idx]->debris = dbrForm ? DYNAMIC_CAST(dbrForm, TESForm, BGSDebris) : NULL;
		}
	}
	return true;
}

bool Cmd_GetNthDestructionStageReplacement_Execute(COMMAND_ARGS)
{
	TESForm *object = NULL;
	UInt32 idx = 0;

	if (ExtractArgs(EXTRACT_ARGS, &object, &idx))
	{
		BGSDestructibleObjectForm *destr = DYNAMIC_CAST(object, TESForm, BGSDestructibleObjectForm);
		if (destr && destr->data && (idx >= 0) && (idx < destr->data->stageCount) && destr->data->stages[idx]->replacement)
		{
			g_string->Assign(PASS_COMMAND_ARGS, destr->data->stages[idx]->replacement->GetModelPath());
			return true;
		}
	}
	g_string->Assign(PASS_COMMAND_ARGS, "");
	return true;
}

bool Cmd_SetNthDestructionStageReplacement_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESForm *object = NULL;
	UInt32 idx = 0;
	char nifPath[512] = {0};
	
	if (ExtractArgs(EXTRACT_ARGS, &object, &idx, &nifPath))
	{
		BGSDestructibleObjectForm *destr = DYNAMIC_CAST(object, TESForm, BGSDestructibleObjectForm);
		if (destr && destr->data && (idx >= 0) && (idx < destr->data->stageCount) && destr->data->stages[idx]->replacement)
		{
			destr->data->stages[idx]->replacement->SetModelPath(nifPath);
			*result = 1;
		}
	}
	return true;
}
