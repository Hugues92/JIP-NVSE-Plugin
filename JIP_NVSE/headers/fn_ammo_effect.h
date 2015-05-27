#pragma once

DEFINE_COMMAND_PLUGIN(GetNumAmmoEffects, "None", 0, 1, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(GetNthAmmoEffect, "None", 0, 2, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(AddAmmoEffect, "None", 0, 2, kParams_JIP_TwoForms);
DEFINE_COMMAND_PLUGIN(RemoveAmmoEffect, "None", 0, 2, kParams_JIP_TwoForms);
DEFINE_COMMAND_PLUGIN(GetAmmoEffectTraitNumeric, "None", 0, 2, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(SetAmmoEffectTraitNumeric, "None", 0, 3, kParams_JIP_OneForm_OneInt_OneFloat);

bool Cmd_GetNumAmmoEffects_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESForm *form = NULL;

	if (ExtractArgs(EXTRACT_ARGS, &form) && (form = form->TryGetREFRParent()))
	{
		TESAmmo *ammo = DYNAMIC_CAST(form, TESForm, TESAmmo);
		if (ammo) *result = ammo->effectList.Count();
	}
	return true;
}

bool Cmd_GetNthAmmoEffect_Execute(COMMAND_ARGS)
{
	UInt32 *refResult = (UInt32*)result, idx = 0;
	*refResult = 0;
	TESForm *form = NULL;

	if (ExtractArgs(EXTRACT_ARGS, &form, &idx) && (idx >= 0) && (form = form->TryGetREFRParent()))
	{
		TESAmmo *ammo = DYNAMIC_CAST(form, TESForm, TESAmmo);
		if (ammo && (idx < ammo->effectList.Count()))
		{
			TESAmmoEffect *effect = ammo->effectList.GetNthItem(idx);
			if (effect) *refResult = effect->refID;
		}
	}
	return true;
}

SInt8 GetEffectIndex(tList<TESAmmoEffect> &lst, TESAmmoEffect *effect)
{
	for (SInt8 idx = 0; idx < lst.Count(); idx++)
	{
		if (lst.GetNthItem(idx) == effect) return idx;
	}
	return -1;
}

bool Cmd_AddAmmoEffect_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESForm *form = NULL, *effForm = NULL;

	if (ExtractArgs(EXTRACT_ARGS, &form, &effForm) && (form = form->TryGetREFRParent()))
	{
		TESAmmo *ammo = DYNAMIC_CAST(form, TESForm, TESAmmo);
		TESAmmoEffect *effect = DYNAMIC_CAST(effForm, TESForm, TESAmmoEffect);
		if (ammo && effect && (GetEffectIndex(ammo->effectList, effect) == -1))
		{
			ammo->effectList.AddAt(effect, eListEnd);
			*result = 1;
		}
	}
	return true;
}

bool Cmd_RemoveAmmoEffect_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESForm *form = NULL, *effForm = NULL;

	if (ExtractArgs(EXTRACT_ARGS, &form, &effForm) && (form = form->TryGetREFRParent()))
	{
		TESAmmo *ammo = DYNAMIC_CAST(form, TESForm, TESAmmo);
		TESAmmoEffect *effect = DYNAMIC_CAST(effForm, TESForm, TESAmmoEffect);
		if (ammo && effect)
		{
			SInt8 idx = GetEffectIndex(ammo->effectList, effect);
			if (idx >= 0)
			{
				ammo->effectList.RemoveNth(idx);
				*result = 1;
			}
		}
	}
	return true;
}

bool Cmd_GetAmmoEffectTraitNumeric_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESForm *form = NULL;
	UInt32 traitID = 0;

	if (ExtractArgs(EXTRACT_ARGS, &form, &traitID) && ValidInput(traitID, 2))
	{
		TESAmmoEffect *effect = DYNAMIC_CAST(form, TESForm, TESAmmoEffect);
		if (effect)
		{
			if (!traitID) *result = effect->type;
			else if (traitID == 1) *result = effect->operation;
			else *result = effect->value;
		}
	}
	return true;
}

bool Cmd_SetAmmoEffectTraitNumeric_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESForm *form = NULL;
	UInt32 traitID = 0;
	float val = 0;

	if (ExtractArgs(EXTRACT_ARGS, &form, &traitID, &val) && ValidInput(traitID, 2) && (val >= 0))
	{
		TESAmmoEffect *effect = DYNAMIC_CAST(form, TESForm, TESAmmoEffect);
		if (effect)
		{
			if (!traitID)
			{
				*result = effect->type;
				if (val < 6) effect->type = val;
			}
			else if (traitID == 1)
			{
				*result = effect->operation;
				if (val < 3) effect->operation = val;
			}
			else
			{
				*result = effect->value;
				effect->value = val;
			}
		}
	}
	return true;
}
