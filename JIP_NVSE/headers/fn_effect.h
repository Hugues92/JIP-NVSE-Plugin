#pragma once

DEFINE_COMMAND_PLUGIN(GetNumEffects, "None", 0, 1, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(GetNthEffectBase, "None", 0, 2, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(SetNthEffectBase, "None", 0, 3, kParams_JIP_OneForm_OneInt_OneMagicEffect);
DEFINE_COMMAND_PLUGIN(GetNthEffectTraitNumeric, "None", 0, 3, kParams_JIP_OneForm_TwoInts);
DEFINE_COMMAND_PLUGIN(SetNthEffectTraitNumeric, "None", 0, 4, kParams_JIP_OneForm_ThreeInts);
DEFINE_COMMAND_PLUGIN(AddNewEffect, "None", 0, 6, kParams_JIP_OneForm_OneEffect_TwoInts_TwoOptionalInts);
DEFINE_COMMAND_PLUGIN(RemoveNthEffect, "None", 0, 2, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(SetObjectEffect, "None", 0, 2, kParams_JIP_TwoForms);
DEFINE_COMMAND_PLUGIN(GetNumActorEffects, "None", 0, 1, kParams_OneOptionalActorBase);
DEFINE_COMMAND_PLUGIN(GetNthActorEffect, "None", 0, 2, kParams_JIP_OneInt_OneOptionalActorBase);
DEFINE_COMMAND_PLUGIN(GetActorUnarmedEffect, "None", 0, 1, kParams_OneOptionalActorBase);
DEFINE_COMMAND_PLUGIN(SetActorUnarmedEffect, "None", 0, 2, kParams_JIP_OneForm_OneOptionalActorBase);

EffectList *GetEffectList(TESForm *form)
{
	EffectItemList *effList = DYNAMIC_CAST(form, TESForm, EffectItemList);
	return effList ? &effList->list : NULL;
}

bool Cmd_GetNumEffects_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESForm *form = NULL;

	if (ExtractArgs(EXTRACT_ARGS, &form))
	{
		EffectList *effList = GetEffectList(form);
		if (effList) *result = effList->Count();
	}
	return true;
}

bool Cmd_GetNthEffectBase_Execute(COMMAND_ARGS)
{
	UInt32 *refResult = (UInt32*)result, idx = 0;
	*refResult = 0;
	TESForm *form = NULL;

	if (ExtractArgs(EXTRACT_ARGS, &form, &idx) && (idx >= 0))
	{
		EffectList *effList = GetEffectList(form);
		if (effList && (idx < effList->Count()))
		{
			EffectItem *effItem = effList->GetNthItem(idx);
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

	if (ExtractArgs(EXTRACT_ARGS, &form, &idx, &effSetting) && (idx >= 0))
	{
		EffectList *effList = GetEffectList(form);
		if (effList && (idx < effList->Count()))
		{
			EffectItem *effItem = effList->GetNthItem(idx);
			if (effItem)
			{
				if (effItem->setting) *refResult = effItem->setting->refID;
				effItem->setting = effSetting;
				effItem->actorValueOrOther = effSetting->actorVal;
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

	if (ExtractArgs(EXTRACT_ARGS, &form, &idx, &traitID) && ValidInput(traitID, 3) && (idx >= 0))
	{
		EffectList *effList = GetEffectList(form);
		if (effList && (idx < effList->Count()))
		{
			EffectItem *effItem = effList->GetNthItem(idx);
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

	if (ExtractArgs(EXTRACT_ARGS, &form, &idx, &traitID, &val) && ValidInput(traitID, 2) && (idx >= 0))
	{
		EffectList *effList = GetEffectList(form);
		if (effList && (idx < effList->Count()))
		{
			EffectItem *effItem = effList->GetNthItem(idx);
			if (effItem)
			{
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
				else
				{
					*result = effItem->duration;
					effItem->duration = val;
				}
			}
		}
	}
	return true;
}

bool Cmd_AddNewEffect_Execute(COMMAND_ARGS)
{
	*result = -1;
	TESForm *form = NULL;
	EffectSetting *effSetting = NULL;
	UInt32 duration = 0, magnitude = 0, range = 0, area = 0;

	if (ExtractArgs(EXTRACT_ARGS, &form, &effSetting, &duration, &magnitude, &range, &area) && ValidInput(range, 2))
	{
		EffectList *effList = GetEffectList(form);
		if (!effList) return true;
		if (form->typeID == kFormType_AlchemyItem) area = range = 0;
		else if (form->typeID == kFormType_Enchantment)
		{
			EnchantmentItem *enchItem = DYNAMIC_CAST(form, TESForm, EnchantmentItem);
			if (!enchItem) return true;
			if (enchItem->type == 2) range = 1;
			else area = range = 0;
		}
		else
		{
			SpellItem *spelItem = DYNAMIC_CAST(form, TESForm, SpellItem);
			if (!spelItem) return true;
			if ((spelItem->type == 1) || (spelItem->type == 4) || (spelItem->type == 10)) range = 0;
			if (!range) area = 0;
		}
		if (!(effSetting->effectFlags & (16 << range))) return true;
		EffectItem *effItem = (EffectItem*)FormHeap_Allocate(0x24);
		memset(effItem, 0, 0x24);
		effItem->setting = effSetting;
		effItem->range = range;
		effItem->area = area;
		effItem->duration = duration;
		effItem->magnitude = magnitude;
		effItem->actorValueOrOther = effSetting->actorVal;
		effList->AddAt(effItem, eListEnd);
		*result = effList->Count();
	}
	return true;
}

bool Cmd_RemoveNthEffect_Execute(COMMAND_ARGS)
{
	*result = -1;
	TESForm *form = NULL;
	UInt32 idx = 0;

	if (ExtractArgs(EXTRACT_ARGS, &form, &idx) && (idx >= 0))
	{
		EffectList *effList = GetEffectList(form);
		if (effList && (idx < effList->Count()))
		{
			EffectItem *effItem = effList->GetNthItem(idx);
			if (effItem) FormHeap_Free(effItem);
			while (idx--) effList->ReplaceNth(idx + 1, effList->GetNthItem(idx));
			effList->RemoveNth(0);
			*result = effList->Count();
		}
	}
	return true;
}

bool Cmd_SetObjectEffect_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESForm *form = NULL, *effForm = NULL;

	if (ExtractArgs(EXTRACT_ARGS, &form, &effForm) && (form = form->TryGetREFRParent()))
	{
		TESEnchantableForm *enchantable = DYNAMIC_CAST(form, TESForm, TESEnchantableForm);
		if (!enchantable) return true;
		EnchantmentItem *effect = DYNAMIC_CAST(effForm, TESForm, EnchantmentItem);
		if (!effect) return true;
		if (form->typeID == kFormType_Armor)
		{
			if (effect->type != 3) return true;
		}
		else if (effect->type != 2) return true;
		enchantable->enchantItem = effect;
		*result = 1;
	}
	return true;
}

bool Cmd_GetNumActorEffects_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESActorBase *actorBase = NULL;

	if (thisObj) actorBase = DYNAMIC_CAST(thisObj->baseForm, TESForm, TESActorBase);
	else ExtractArgs(EXTRACT_ARGS, &actorBase);
	if (actorBase) *result = actorBase->spellList.spellList.Count();
	return true;
}

bool Cmd_GetNthActorEffect_Execute(COMMAND_ARGS)
{
	UInt32 *refResult = (UInt32*)result, idx = 0;
	*refResult = 0;
	TESActorBase *actorBase = NULL;

	if (ExtractArgs(EXTRACT_ARGS, &idx, &actorBase))
	{
		if (thisObj) actorBase = DYNAMIC_CAST(thisObj->baseForm, TESForm, TESActorBase);
		if (actorBase && (idx < actorBase->spellList.spellList.Count()))
		{
			SpellItem *splItem = actorBase->spellList.spellList.GetNthItem(idx);
			if (splItem) *refResult = splItem->refID;
		}
	}
	return true;
}

bool Cmd_GetActorUnarmedEffect_Execute(COMMAND_ARGS)
{
	UInt32 *refResult = (UInt32*)result;
	*refResult = 0;
	TESActorBase *actorBase = NULL;
	BGSTouchSpellForm *touchSpell = NULL;

	if (thisObj) touchSpell = DYNAMIC_CAST(thisObj->baseForm, TESForm, BGSTouchSpellForm);
	else if (ExtractArgs(EXTRACT_ARGS, &actorBase) && actorBase) touchSpell = &actorBase->touchSpell;
	if (touchSpell && touchSpell->unarmedEffect) *refResult = touchSpell->unarmedEffect->refID;
	return true;
}

bool Cmd_SetActorUnarmedEffect_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESForm *effForm = NULL;
	TESActorBase *actorBase = NULL;

	if (ExtractArgs(EXTRACT_ARGS, &effForm, &actorBase))
	{
		BGSTouchSpellForm *touchSpell = NULL;
		if (thisObj) touchSpell = DYNAMIC_CAST(thisObj->baseForm, TESForm, BGSTouchSpellForm);
		else if (actorBase) touchSpell = &actorBase->touchSpell;
		if (!touchSpell) return true;
		if (effForm->typeID == kFormType_Enchantment)
		{
			EnchantmentItem *enchItem = DYNAMIC_CAST(effForm, TESForm, EnchantmentItem);
			if (!enchItem || (enchItem->type != 2)) return true;
		}
		else if (effForm->typeID == kFormType_Spell)
		{
			SpellItem *spelItem = DYNAMIC_CAST(effForm, TESForm, SpellItem);
			if (!spelItem || spelItem->type) return true;
		}
		touchSpell->unarmedEffect = effForm;
		*result = 1;
	}
	return true;
}
