#pragma once

DEFINE_COMMAND_PLUGIN(GetNumEffects, "None", 0, 1, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(GetNthEffectBase, "None", 0, 2, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(SetNthEffectBase, "None", 0, 3, kParams_JIP_OneForm_OneInt_OneMagicEffect);
DEFINE_COMMAND_PLUGIN(GetNthEffectTraitNumeric, "None", 0, 3, kParams_JIP_OneForm_TwoInts);
DEFINE_COMMAND_PLUGIN(SetNthEffectTraitNumeric, "None", 0, 4, kParams_JIP_OneForm_ThreeInts);

tList<EffectItem> *GetEffectList(TESForm *form)
{
	MagicItemForm *magicItem = DYNAMIC_CAST(form, TESForm, MagicItemForm);
	if (magicItem) return &magicItem->magicItem.list.list;

	AlchemyItem *alchItem = DYNAMIC_CAST(form, TESForm, AlchemyItem);
	if (alchItem) return &alchItem->effects.list;

	return NULL;
}

bool Cmd_GetNumEffects_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESForm *form = NULL;

	if(ExtractArgs(EXTRACT_ARGS, &form))
	{
		tList<EffectItem> *effectList = GetEffectList(form);
		if (effectList) *result = effectList->Count();
	}
	return true;
}

bool Cmd_GetNthEffectBase_Execute(COMMAND_ARGS)
{
	UInt32 *refResult = (UInt32*)result, idx = 0;
	*refResult = 0;
	TESForm *form = NULL;

	if(ExtractArgs(EXTRACT_ARGS, &form, &idx))
	{
		tList<EffectItem> *effectList = GetEffectList(form);
		if (effectList && (idx < effectList->Count()))
		{
			EffectItem *effItem = effectList->GetNthItem(idx);
			if (effItem && effItem->setting) *refResult = effItem->setting->refID;
		}
	}
	return true;
}

bool Cmd_SetNthEffectBase_Execute(COMMAND_ARGS)
{
	UInt32 *refResult = (UInt32*)result, idx = 0;
	*refResult = 0;
	TESForm *form = NULL;
	EffectSetting *effSetting = NULL;

	if(ExtractArgs(EXTRACT_ARGS, &form, &idx, &effSetting))
	{
		tList<EffectItem> *effectList = GetEffectList(form);
		if (effectList && (idx < effectList->Count()))
		{
			EffectItem *effItem = effectList->GetNthItem(idx);
			if (effItem)
			{
				if (effItem->setting) *refResult = effItem->setting->refID;
				effItem->setting = effSetting;
			}
		}
	}
	return true;
}

bool Cmd_GetNthEffectTraitNumeric_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESForm *form = NULL;
	UInt32 idx = 0, traitID = 0;

	if(ExtractArgs(EXTRACT_ARGS, &form, &idx, &traitID) && ValidInput(traitID, 3))
	{
		tList<EffectItem> *effectList = GetEffectList(form);
		if (effectList && (idx < effectList->Count()))
		{
			EffectItem *effItem = effectList->GetNthItem(idx);
			if (effItem)
			{
				if (!traitID) *result = effItem->magnitude;
				else if (traitID == 1) *result = effItem->area;
				else if (traitID == 2) *result = effItem->duration;
				else *result = effItem->range;
			}
		}
	}
	return true;
}

bool Cmd_SetNthEffectTraitNumeric_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESForm *form = NULL;
	UInt32 idx = 0, traitID = 0, val = 0;

	if(ExtractArgs(EXTRACT_ARGS, &form, &idx, &traitID, &val) && ValidInput(traitID, 3))
	{
		tList<EffectItem> *effectList = GetEffectList(form);
		if (effectList && (idx < effectList->Count()))
		{
			EffectItem *effItem = effectList->GetNthItem(idx);
			if (effItem)
			{
				if (val < 0) val = 0;
				if (!traitID)
				{
					*result = effItem->magnitude;
					effItem->magnitude = val;
				}
				else if (traitID == 1)
				{
					*result = effItem->area;
					effItem->area = val;
				}
				else if (traitID == 2)
				{
					*result = effItem->duration;
					effItem->duration = val;
				}
				else if (val < 3)
				{
					*result = effItem->range;
					effItem->range = val;
				}
			}
		}
	}
	return true;
}
