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
DEFINE_COMMAND_PLUGIN(AddDestructionStage, "None", 0, 4, kParams_JIP_OneObject_ThreeInts);
DEFINE_COMMAND_PLUGIN(RemoveDestructionStage, "None", 0, 2, kParams_JIP_OneObject_OneInt);

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
		if (destr && destr->data && destr->data->stages && (idx >= 0) && (idx < destr->data->stageCount))
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
		if (destr && destr->data && destr->data->stages && (idx >= 0) && (idx < destr->data->stageCount))
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
		if (destr && destr->data && destr->data->stages && (idx >= 0) && (idx < destr->data->stageCount) && destr->data->stages[idx]->explosion)
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
		if (destr && destr->data && destr->data->stages && (idx >= 0) && (idx < destr->data->stageCount))
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
		if (destr && destr->data && destr->data->stages && (idx >= 0) && (idx < destr->data->stageCount) && destr->data->stages[idx]->debris)
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
		if (destr && destr->data && destr->data->stages && (idx >= 0) && (idx < destr->data->stageCount))
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
		if (destr && destr->data && destr->data->stages && (idx >= 0) && (idx < destr->data->stageCount) && destr->data->stages[idx]->replacement)
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
		if (destr && destr->data && destr->data->stages && (idx >= 0) && (idx < destr->data->stageCount))
		{
			TESModelTextureSwap *rplc = destr->data->stages[idx]->replacement;
			if (!rplc)
			{
				destr->data->stages[idx]->replacement = rplc = (TESModelTextureSwap*)FormHeap_Allocate(sizeof(TESModelTextureSwap));
				memset(rplc, 0, sizeof(TESModelTextureSwap));
				*(UInt32*)rplc = s_TESModelTextureSwapVtbl;
			}
			rplc->SetModelPath(nifPath);
			*result = 1;
		}
	}
	return true;
}

typedef std::vector<DestructionStage*> DstrStagesArr;

void GetDstrStagesArr(DestructibleData *data, DstrStagesArr &arr)
{
	for (UInt32 idx = 0; idx < data->stageCount; idx++)
	{
		if (data->stages[idx]) arr.push_back(data->stages[idx]);
	}
}

bool Cmd_AddDestructionStage_Execute(COMMAND_ARGS)
{
	*result = -1;
	TESForm *object = NULL;
	SInt32 idx = 0;
	UInt32 health = 0, dmgStg = 0;

	if (ExtractArgs(EXTRACT_ARGS, &object, &idx, &health, &dmgStg))
	{
		BGSDestructibleObjectForm *destr = DYNAMIC_CAST(object, TESForm, BGSDestructibleObjectForm);
		if (destr)
		{
			SInt32 count = (destr->data && destr->data->stages) ? destr->data->stageCount : 0;
			if (idx > count) return true;

			if (idx < 0) idx = count;
			*result = idx;
			DstrStagesArr stgArr;
			if (count > 0) GetDstrStagesArr(destr->data, stgArr);
			DestructionStage *newStage = (DestructionStage*)FormHeap_Allocate(0x18);
			memset(newStage, 0, 0x18);
			newStage->healthPrc = health;
			newStage->dmgStage = dmgStg;
			stgArr.insert(stgArr.begin() + idx, newStage);
			count = stgArr.size() * 4;

			if (!destr->data)
			{
				destr->data = (DestructibleData*)FormHeap_Allocate(0xC);
				memset(destr->data, 0, 0xC);
				*(UInt16*)&destr->data->unk06 = 0x160D;
			}
			else if (destr->data->stages) FormHeap_Free(destr->data->stages);

			destr->data->stages = (DestructionStage**)FormHeap_Allocate(count);
			memcpy(destr->data->stages, stgArr.data(), count);
			destr->data->stageCount++;
		}
	}
	return true;
}

bool Cmd_RemoveDestructionStage_Execute(COMMAND_ARGS)
{
	*result = -1;
	TESForm *object = NULL;
	UInt32 idx = 0;

	if (ExtractArgs(EXTRACT_ARGS, &object, &idx))
	{
		BGSDestructibleObjectForm *destr = DYNAMIC_CAST(object, TESForm, BGSDestructibleObjectForm);
		if (destr && destr->data && destr->data->stages)
		{
			UInt32 count = destr->data->stageCount;
			if (idx >= count) return true;

			*result = idx;
			DstrStagesArr stgArr;
			GetDstrStagesArr(destr->data, stgArr);
			FormHeap_Free(stgArr[idx]);
			stgArr.erase(stgArr.begin() + idx);

			if (stgArr.empty())
			{
				FormHeap_Free(destr->data->stages);
				destr->data->stages = NULL;
			}
			else
			{
				stgArr.push_back(0);
				memcpy(destr->data->stages, stgArr.data(), stgArr.size() * 4);
			}
			destr->data->stageCount--;
		}
	}
	return true;
}
