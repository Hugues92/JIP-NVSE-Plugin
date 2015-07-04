#pragma once

DEFINE_COMMAND_PLUGIN(GetActorTemplate, "None", 0, 1, kParams_OneOptionalActorBase);
DEFINE_COMMAND_PLUGIN(GetLeveledActorBase, "None", 1, 0, NULL);
DEFINE_COMMAND_PLUGIN(GetCreatureDamage, "None", 0, 1, kParams_OneOptionalActorBase);
DEFINE_COMMAND_PLUGIN(SetCreatureDamage, "None", 0, 2, kParams_JIP_OneInt_OneOptionalActorBase);
DEFINE_CMD_ALT_COND_PLUGIN(GetIsPoisoned, "None", 1, 0, NULL);
DEFINE_COMMAND_PLUGIN(GetFollowers, "None", 1, 0, NULL);
DEFINE_COMMAND_PLUGIN(GetActorLevelingData, "None", 0, 2, kParams_JIP_OneInt_OneOptionalActorBase);
DEFINE_COMMAND_PLUGIN(SetActorLevelingData, "None", 0, 3, kParams_JIP_OneInt_OneFloat_OneOptionalActorBase);
DEFINE_COMMAND_PLUGIN(GetActorVoiceType, "None", 0, 1, kParams_OneOptionalActorBase);
DEFINE_COMMAND_PLUGIN(SetActorVoiceType, "None", 0, 2, kParams_JIP_OneForm_OneOptionalActorBase);
DEFINE_COMMAND_PLUGIN(GetCreatureReach, "None", 0, 1, kParams_OneOptionalActorBase);
DEFINE_COMMAND_PLUGIN(GetIsImmobile, "None", 0, 1, kParams_OneOptionalActorBase);
DEFINE_COMMAND_PLUGIN(PickFromList, "None", 1, 3, kParams_JIP_OneList_TwoOptionalInts);
DEFINE_COMMAND_PLUGIN(SetRefEssential, "None", 1, 1, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(ToggleCreatureModel, "None", 0, 3, kParams_JIP_OneString_OneInt_OneOptionalActorBase);
DEFINE_COMMAND_PLUGIN(CreatureHasModel, "None", 0, 2, kParams_JIP_OneString_OneOptionalActorBase);
DEFINE_COMMAND_PLUGIN(GetCreatureModels, "None", 0, 1, kParams_OneOptionalActorBase);

bool Cmd_GetActorTemplate_Execute(COMMAND_ARGS)
{
	UInt32 *refResult = (UInt32*)result;
	*refResult = 0;
	TESForm *form = NULL;

	if (thisObj) form = thisObj->baseForm;
	else ExtractArgs(EXTRACT_ARGS, &form);

	if (form)
	{
		TESActorBaseData *baseData = DYNAMIC_CAST(form, TESForm, TESActorBaseData);
		if (baseData && baseData->templateActor) *refResult = baseData->templateActor->refID;
	}
	if (IsConsoleMode()) Console_Print("Actor Template >> %08X", *refResult);
	return true;
}

bool Cmd_GetLeveledActorBase_Execute(COMMAND_ARGS)
{
	UInt32 *refResult = (UInt32*)result;
	*refResult = 0;

	if (thisObj)
	{
		ExtraLeveledCreature *xLvlCre = (ExtraLeveledCreature*)GetByTypeCast(thisObj->extraDataList, LeveledCreature);
		if (xLvlCre && xLvlCre->form) *refResult = xLvlCre->form->refID;
	}
	if (IsConsoleMode()) Console_Print("Leveled-Actor Base >> %08X", *refResult);
	return true;
}

bool Cmd_GetCreatureDamage_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESForm *form = NULL;

	if (thisObj) form = thisObj->baseForm;
	else ExtractArgs(EXTRACT_ARGS, &form);

	if (form)
	{
		TESCreature *creature = DYNAMIC_CAST(form, TESForm, TESCreature);
		if (creature) *result = creature->attackDmg.damage;
	}
	if (IsConsoleMode()) Console_Print("Creature Damage >> %.0f", *result);
	return true;
}

bool Cmd_SetCreatureDamage_Execute(COMMAND_ARGS)
{
	*result = 0;
	UInt32 dmg = 0;
	TESForm *form = NULL;

	if (ExtractArgs(EXTRACT_ARGS, &dmg, &form) && (dmg >= 0))
	{
		if (thisObj) form = thisObj->baseForm;
		if (form)
		{
			TESCreature *creature = DYNAMIC_CAST(form, TESForm, TESCreature);
			if (creature)
			{
				*result = creature->attackDmg.damage;
				creature->attackDmg.damage = dmg;
			}
		}
	}
	return true;
}

bool Cmd_GetIsPoisoned_Eval(COMMAND_ARGS_EVAL)
{
	*result = 0;
	if (thisObj)
	{
		Actor *actor = DYNAMIC_CAST(thisObj, TESObjectREFR, Actor);
		if (actor)
		{
			UInt32 count = actor->magicTarget.GetEffectList()->Count();
			ActiveEffect *effect;
			for (UInt32 idx = 0; idx < count; idx++)
			{
				if ((effect = actor->magicTarget.GetEffectList()->GetNthItem(idx)) && (effect->spellType == 5))
				{
					*result = 1;
					return true;
				}
			}
		}
	}
	return true;
}

bool Cmd_GetIsPoisoned_Execute(COMMAND_ARGS)
{
	Cmd_GetIsPoisoned_Eval(thisObj, NULL, NULL, result);
	if (IsConsoleMode()) Console_Print("Poisoned >> %.0f", *result);
	return true;
}

bool Cmd_GetFollowers_Execute(COMMAND_ARGS)
{
	NVSEArrayVarInterface::Array *followersArr = g_array->CreateArray(NULL, 0, scriptObj);
	g_array->AssignCommandResult(followersArr, result);
	if (thisObj)
	{
		ExtraFollower *xFollower = (ExtraFollower*)GetByTypeCast(thisObj->extraDataList, Follower);
		if (xFollower)
		{
			NVSEArrayVarInterface::Element *elem;
			for (auto iter = xFollower->followers->Begin(); !iter.End(); ++iter)
			{
				elem = new NVSEArrayVarInterface::Element(iter.Get());
				g_array->AppendElement(followersArr, *elem);
			}
		}
	}
	return true;
}

bool Cmd_GetActorLevelingData_Execute(COMMAND_ARGS)
{
	*result = 0;
	UInt32 valID = 0;
	TESActorBase *actorBase = NULL;

	if (ExtractArgs(EXTRACT_ARGS, &valID, &actorBase) && ValidInput(valID, 2))
	{
		if (!actorBase && thisObj) actorBase = DYNAMIC_CAST(thisObj->baseForm, TESForm, TESActorBase);

		if (actorBase)
		{
			if (valID == 1) *result = actorBase->baseData.calcMin;
			else if (valID == 2) *result = actorBase->baseData.calcMax;
			else if (actorBase->baseData.flags & 128) *result = actorBase->baseData.level / 1000.0;
			else *result = actorBase->baseData.level;
		}
	}
	return true;
}

bool Cmd_SetActorLevelingData_Execute(COMMAND_ARGS)
{
	*result = 0;
	UInt32 valID = 0;
	float val = 0;
	TESActorBase *actorBase = NULL;

	if (ExtractArgs(EXTRACT_ARGS, &valID, &val, &actorBase) && ValidInput(valID, 2))
	{
		if (!actorBase && thisObj) actorBase = DYNAMIC_CAST(thisObj->baseForm, TESForm, TESActorBase);

		if (actorBase)
		{
			if (!valID)
			{
				if (actorBase->baseData.flags & 128)
				{
					*result = actorBase->baseData.level / 1000.0;
					if (val < 0.01) val = 10;
					else val *= 1000;
				}
				else
				{
					*result = actorBase->baseData.level;
					if (val < 1) val = 1;
				}
				actorBase->baseData.level = val;
			}
			else if (valID == 1)
			{
				*result = actorBase->baseData.calcMin;
				if (val < 1) val = 1;
				actorBase->baseData.calcMin = val;
				if (actorBase->baseData.calcMax < val) actorBase->baseData.calcMax = val;
			}
			else
			{
				*result = actorBase->baseData.calcMax;
				if (val < 1) val = 1;
				actorBase->baseData.calcMax = val;
				if (actorBase->baseData.calcMin > val) actorBase->baseData.calcMin = val;
			}
		}
	}
	return true;
}

bool Cmd_GetActorVoiceType_Execute(COMMAND_ARGS)
{
	UInt32 *refResult = (UInt32*)result;
	*refResult = 0;
	TESActorBase *actorBase = NULL;
	TESActorBaseData *baseData = NULL;

	if (thisObj) baseData = DYNAMIC_CAST(thisObj->baseForm, TESForm, TESActorBaseData);
	else if (ExtractArgs(EXTRACT_ARGS, &actorBase) && actorBase) baseData = &actorBase->baseData;
	if (baseData && baseData->voiceType) *refResult = baseData->voiceType->refID;
	return true;
}

bool Cmd_SetActorVoiceType_Execute(COMMAND_ARGS)
{
	UInt32 *refResult = (UInt32*)result;
	*refResult = 0;
	TESForm *form = NULL;
	TESActorBase *actorBase = NULL;

	if (ExtractArgs(EXTRACT_ARGS, &form, &actorBase))
	{
		TESActorBaseData *baseData = NULL;
		if (thisObj) baseData = DYNAMIC_CAST(thisObj->baseForm, TESForm, TESActorBaseData);
		else if (actorBase) baseData = &actorBase->baseData;

		if (baseData)
		{
			BGSVoiceType *voice = DYNAMIC_CAST(form, TESForm, BGSVoiceType);
			if (voice)
			{
				if (baseData->voiceType) *refResult = baseData->voiceType->refID;
				baseData->voiceType = voice;
			}
		}
	}
	return true;
}

bool Cmd_GetCreatureReach_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESForm *form = NULL;

	if (thisObj) form = thisObj->baseForm;
	else if (!ExtractArgs(EXTRACT_ARGS, &form) || !form) return true;
	TESCreature *creature = DYNAMIC_CAST(form, TESForm, TESCreature);
	if (creature) *result = creature->attackReach;
	return true;
}

bool Cmd_GetIsImmobile_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESForm *form = NULL;

	if (thisObj) form = thisObj->baseForm;
	else ExtractArgs(EXTRACT_ARGS, &form);

	if (form && (form->typeID == 43))
	{
		TESActorBaseData *baseData = DYNAMIC_CAST(form, TESForm, TESActorBaseData);
		if (baseData) *result = (baseData->flags & 0x800000) ? 1 : 0;
	}
	return true;
}

bool Cmd_PickFromList_Execute(COMMAND_ARGS)
{
	UInt32 *refResult = (UInt32*)result;
	*refResult = 0;
	BGSListForm *lst = NULL;
	UInt32 start = 0, len = 0, itmID;

	if (thisObj && thisObj->GetContainer() && ExtractArgs(EXTRACT_ARGS, &lst, &start, &len) && (start >= 0) && (len >= 0))
	{
		SInt32 count = lst->Count();
		if (count > start)
		{
			len = len ? min(count, start + len) : count;
			for (UInt16 idx = start; idx < len; idx++)
			{
				itmID = lst->GetNthForm(idx)->refID;
				GetItemByRefID(thisObj, itmID, &count);
				if (count)
				{
					*refResult = itmID;
					return true;
				}
			}
		}
	}
	return true;
}

bool Cmd_SetRefEssential_Execute(COMMAND_ARGS)
{
	*result = 0;
	UInt32 flag = 0;

	if (thisObj && ExtractArgs(EXTRACT_ARGS, &flag))
	{
		TESActorBaseData *baseData = DYNAMIC_CAST(thisObj->baseForm, TESForm, TESActorBaseData);
		if (baseData)
		{
			*result = (baseData->flags & 2) ? 1 : 0;
			flag ? (baseData->flags |= 2) : (baseData->flags &= ~2);
		}
	}
	return true;
}

SInt32 TESModelList::GetIndex(char *path)
{
	NameToLower(path);
	SInt32 idx = 0;
	for (auto iter = modelList.Begin(); !iter.End(); ++iter)
	{
		NameToLower(iter.Get());
		if (!strcmp(iter.Get(), path)) return idx;
		idx++;
	}
	return -1;
}

bool TESModelList::RemoveEntry(char *nifToRemove)
{
	SInt32 idx = GetIndex(nifToRemove);
	if (idx == -1) return false;
	nifToRemove = modelList.RemoveNth(idx);
	FormHeap_Free(nifToRemove);
	count--;
	return true;
}

bool TESModelList::AddEntry(char *nifToAdd)
{
	if (GetIndex(nifToAdd) != -1) return false;
	UInt32 newLen = strlen(nifToAdd) + 1;
	char *newStr = (char*)FormHeap_Allocate(newLen);
	strcpy_s(newStr, newLen, nifToAdd);
	modelList.AddAt(newStr, eListEnd);
	count++;
	return true;
}

bool Cmd_ToggleCreatureModel_Execute(COMMAND_ARGS)
{
	*result = 0;
	char nifPath[256] = {0};
	UInt32 enable = 0;
	TESActorBase *actorBase = NULL;

	if (ExtractArgs(EXTRACT_ARGS, &nifPath, &enable, &actorBase))
	{
		TESCreature *cretr = NULL;
		if (actorBase) cretr = DYNAMIC_CAST(actorBase, TESActorBase, TESCreature);
		else if (thisObj) cretr = DYNAMIC_CAST(thisObj->baseForm, TESForm, TESCreature);
		if (cretr) *result = (enable ? cretr->modelList.AddEntry(nifPath) : cretr->modelList.RemoveEntry(nifPath)) ? 1 : 0;
	}
	return true;
}

bool Cmd_CreatureHasModel_Execute(COMMAND_ARGS)
{
	*result = 0;
	char nifPath[256] = {0};
	TESActorBase *actorBase = NULL;

	if (ExtractArgs(EXTRACT_ARGS, &nifPath, &actorBase))
	{
		TESCreature *cretr = NULL;
		if (actorBase) cretr = DYNAMIC_CAST(actorBase, TESActorBase, TESCreature);
		else if (thisObj) cretr = DYNAMIC_CAST(thisObj->baseForm, TESForm, TESCreature);
		if (cretr) *result = (cretr->modelList.GetIndex(nifPath) == -1) ? 0 : 1;
	}
	return true;
}

bool Cmd_GetCreatureModels_Execute(COMMAND_ARGS)
{
	NVSEArrayVarInterface::Array *modelsArr = g_array->CreateArray(NULL, 0, scriptObj);
	g_array->AssignCommandResult(modelsArr, result);
	TESActorBase *actorBase = NULL;
	if (ExtractArgs(EXTRACT_ARGS, &actorBase))
	{
		TESCreature *cretr = NULL;
		if (actorBase) cretr = DYNAMIC_CAST(actorBase, TESActorBase, TESCreature);
		else if (thisObj) cretr = DYNAMIC_CAST(thisObj->baseForm, TESForm, TESCreature);
		if (cretr)
		{
			NVSEArrayVarInterface::Element *elem;
			for (auto iter = cretr->modelList.modelList.Begin(); !iter.End(); ++iter)
			{
				elem = new NVSEArrayVarInterface::Element(iter.Get());
				g_array->AppendElement(modelsArr, *elem);
			}
		}
	}
	return true;
}
