#pragma once

DEFINE_COMMAND_PLUGIN(GetWeaponDetectionSoundLevel, "None", 0, 1, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(SetWeaponDetectionSoundLevel, "None", 0, 2, kParams_OneForm_OneInt);
DEFINE_CMD_ALT_COND_PLUGIN(IsEquippedWeaponSilenced, "None", 1, 0, NULL);
DEFINE_CMD_ALT_COND_PLUGIN(IsEquippedWeaponScoped, "None", 1, 0, NULL);
DEFINE_COMMAND_PLUGIN(GetWeaponSound, "None", 0, 2, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(SetWeaponSound, "None", 0, 3, kParams_JIP_OneForm_OneInt_OneOptionalSound);
DEFINE_COMMAND_PLUGIN(SetWeaponItemMod, "None", 0, 3, kParams_JIP_OneForm_OneInt_OneOptionalObjectID);
DEFINE_COMMAND_PLUGIN(SetWeaponItemModEffect, "None", 0, 3, kParams_JIP_OneForm_TwoInts);
DEFINE_COMMAND_PLUGIN(SetWeaponItemModValue, "None", 0, 3, kParams_JIP_OneForm_OneInt_OneFloat);
DEFINE_COMMAND_PLUGIN(GetWeaponModReloadAnim, "None", 0, 1, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(SetWeaponModReloadAnim, "None", 0, 2, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(GetWeaponShellCasingModel, "None", 0, 1, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(SetWeaponShellCasingModel, "None", 0, 2, kParams_JIP_OneForm_OneString);

bool Cmd_GetWeaponDetectionSoundLevel_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESForm *form = NULL;

	if (ExtractArgs(EXTRACT_ARGS, &form) && (form = form->TryGetREFRParent()))
	{
		TESObjectWEAP *weap = DYNAMIC_CAST(form, TESForm, TESObjectWEAP);
		if (weap) *result = weap->unk364;
	}
	return true;
}

bool Cmd_SetWeaponDetectionSoundLevel_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESForm *form = NULL;
	UInt32 level = 0;

	if (ExtractArgs(EXTRACT_ARGS, &form, &level) && (form = form->TryGetREFRParent()) && ValidInput(level, 2))
	{
		TESObjectWEAP *weap = DYNAMIC_CAST(form, TESForm, TESObjectWEAP);
		if (weap)
		{
			*result = weap->unk364;
			weap->unk364 = level;
		}
	}
	return true;
}

class WeaponMatcher : public FormMatcher
{
	UInt32	m_slotMask;
public:
	WeaponMatcher() : m_slotMask(TESBipedModelForm::eSlot_Weapon) {}
	bool Matches(TESForm *form) const
	{
		if (form && form->IsWeapon()) return (TESBipedModelForm::eSlot_Weapon & m_slotMask) != 0;
		return false;
	}
};

static EquipData FindEquipped(TESObjectREFR *thisObj, FormMatcher &matcher)
{
	ExtraContainerChanges *xContainerChanges = (ExtraContainerChanges*)GetByTypeCast(thisObj->extraDataList, ContainerChanges);
	return xContainerChanges ? xContainerChanges->FindEquipped(matcher) : EquipData();
}

UInt8 EquippedWeaponHasMod(TESObjectREFR *thisObj, UInt32 modID)
{
	if (!thisObj || !DYNAMIC_CAST(thisObj, TESObjectREFR, Actor)) return 0;
	WeaponMatcher matcher;
	EquipData eqpData = FindEquipped(thisObj, matcher);
	if (!eqpData.pForm) return 0;
	TESObjectWEAP *weap = DYNAMIC_CAST(eqpData.pForm, TESForm, TESObjectWEAP);
	if (!weap) return 0;
	if (modID == 11)
	{
		if (weap->unk364 == 2) return 1;
	}
	else if (weap->weaponFlags1.IsUnSet(4)) return 0;
	else if (weap->weaponFlags2.IsUnSet(8192)) return 1;
	if (!eqpData.pExtraData) return 0;
	ExtraWeaponModFlags *xWeaponModFlags = (ExtraWeaponModFlags*)eqpData.pExtraData->GetByType(kExtraData_WeaponModFlags);
	if (!xWeaponModFlags) return 0;
	UInt8 modFlags = xWeaponModFlags->flags, idx = 0;
	while (idx < 3)
	{
		if ((modFlags & (1 << idx)) && (weap->effectMods[idx] == modID)) return 2;
		idx++;
	}
	return 0;
}

bool Cmd_IsEquippedWeaponSilenced_Eval(COMMAND_ARGS_EVAL)
{
	*result = EquippedWeaponHasMod(thisObj, 11);
	return true;
}

bool Cmd_IsEquippedWeaponSilenced_Execute(COMMAND_ARGS)
{
	Cmd_IsEquippedWeaponSilenced_Eval(thisObj, NULL, NULL, result);
	if (IsConsoleMode()) Console_Print("Equipped-Weapon Silenced >> %.0f", *result);
	return true;
}

bool Cmd_IsEquippedWeaponScoped_Eval(COMMAND_ARGS_EVAL)
{
	*result = EquippedWeaponHasMod(thisObj, 14);
	return true;
}

bool Cmd_IsEquippedWeaponScoped_Execute(COMMAND_ARGS)
{
	Cmd_IsEquippedWeaponScoped_Eval(thisObj, NULL, NULL, result);
	if (IsConsoleMode()) Console_Print("Equipped-Weapon Scoped >> %.0f", *result);
	return true;
}

bool Cmd_GetWeaponSound_Execute(COMMAND_ARGS)
{
	UInt32 *refResult = (UInt32*)result, type = 0;
	*refResult = 0;
	TESForm *form = NULL;

	if (ExtractArgs(EXTRACT_ARGS, &form, &type) && ValidInput(type, 13) && (form = form->TryGetREFRParent()))
	{
		TESObjectWEAP *weap = DYNAMIC_CAST(form, TESForm, TESObjectWEAP);
		if (weap)
		{
			if (type < 12)
			{
				if (weap->sounds[type]) *refResult = weap->sounds[type]->refID;
			}
			else if (type == 12)
			{
				if (weap->pickupPutdownSounds.pickupSound) *refResult = weap->pickupPutdownSounds.pickupSound->refID;
			}
			else if (weap->pickupPutdownSounds.putdownSound) *refResult = weap->pickupPutdownSounds.putdownSound->refID;
		}
	}
	return true;
}

bool Cmd_SetWeaponSound_Execute(COMMAND_ARGS)
{
	UInt32 *refResult = (UInt32*)result, type = 0;
	*refResult = 0;
	TESForm *form = NULL;
	TESSound *sound = NULL;

	if (ExtractArgs(EXTRACT_ARGS, &form, &type, &sound) && ValidInput(type, 13) && (form = form->TryGetREFRParent()))
	{
		TESObjectWEAP *weap = DYNAMIC_CAST(form, TESForm, TESObjectWEAP);
		if (weap)
		{
			if (type < 12)
			{
				if (weap->sounds[type]) *refResult = weap->sounds[type]->refID;
				weap->sounds[type] = sound;
			}
			else if (type == 12)
			{
				if (weap->pickupPutdownSounds.pickupSound) *refResult = weap->pickupPutdownSounds.pickupSound->refID;
				weap->pickupPutdownSounds.pickupSound = sound;
			}
			else
			{
				if (weap->pickupPutdownSounds.putdownSound) *refResult = weap->pickupPutdownSounds.putdownSound->refID;
				weap->pickupPutdownSounds.putdownSound = sound;
			}
		}
	}
	return true;
}

bool Cmd_SetWeaponItemMod_Execute(COMMAND_ARGS)
{
	UInt32 *refResult = (UInt32*)result, idx = 0;
	*refResult = 0;
	TESForm *weapForm = NULL, *wmodForm = NULL;

	if (ExtractArgs(EXTRACT_ARGS, &weapForm, &idx, &wmodForm) && ValidInput(idx, 3, 1) && (weapForm = weapForm->TryGetREFRParent()))
	{
		TESObjectWEAP *weap = DYNAMIC_CAST(weapForm, TESForm, TESObjectWEAP);
		if (weap)
		{
			TESObjectIMOD *wmod = wmodForm ? DYNAMIC_CAST(wmodForm, TESForm, TESObjectIMOD) : NULL;
			if (idx == 1)
			{
				if (weap->itemMod1) *refResult = weap->itemMod1->refID;
				weap->itemMod1 = wmod;
			}
			else if (idx == 2)
			{
				if (weap->itemMod2) *refResult = weap->itemMod2->refID;
				weap->itemMod2 = wmod;
			}
			else
			{
				if (weap->itemMod3) *refResult = weap->itemMod3->refID;
				weap->itemMod3 = wmod;
			}
		}
	}
	return true;
}

bool Cmd_SetWeaponItemModEffect_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESForm *form = NULL;
	UInt32 idx = 0, type = 0;

	if (ExtractArgs(EXTRACT_ARGS, &form, &idx, &type) && ValidInput(idx, 3, 1) && ValidInput(type, 14, 1) && (type != 5) && (form = form->TryGetREFRParent()))
	{
		TESObjectWEAP *weap = DYNAMIC_CAST(form, TESForm, TESObjectWEAP);
		if (weap)
		{
			*result = weap->effectMods[idx - 1];
			weap->effectMods[idx - 1] = type;
		}
	}
	return true;
}

bool Cmd_SetWeaponItemModValue_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESForm *form = NULL;
	UInt32 idx = 0;
	float val = 0;

	if (ExtractArgs(EXTRACT_ARGS, &form, &idx, &val) && ValidInput(idx, 3, 1) && (form = form->TryGetREFRParent()))
	{
		TESObjectWEAP *weap = DYNAMIC_CAST(form, TESForm, TESObjectWEAP);
		if (weap)
		{
			*result = weap->value1Mod[idx - 1];
			weap->value1Mod[idx - 1] = val;
		}
	}
	return true;
}

bool Cmd_GetWeaponModReloadAnim_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESForm *form = NULL;

	if (ExtractArgs(EXTRACT_ARGS, &form) && (form = form->TryGetREFRParent()))
	{
		TESObjectWEAP *weap = DYNAMIC_CAST(form, TESForm, TESObjectWEAP);
		if (weap) *result = weap->modReloadAnim;
	}
	return true;
}

bool Cmd_SetWeaponModReloadAnim_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESForm *form = NULL;
	UInt32 anim = 0;

	if (ExtractArgs(EXTRACT_ARGS, &form, &anim) && ValidInput(anim, 22) && (form = form->TryGetREFRParent()))
	{
		TESObjectWEAP *weap = DYNAMIC_CAST(form, TESForm, TESObjectWEAP);
		if (weap)
		{
			*result = weap->modReloadAnim;
			weap->modReloadAnim = anim;
		}
	}
	return true;
}

bool Cmd_GetWeaponShellCasingModel_Execute(COMMAND_ARGS)
{
	TESForm *form = NULL;

	if (ExtractArgs(EXTRACT_ARGS, &form) && (form = form->TryGetREFRParent()))
	{
		TESObjectWEAP *weap = DYNAMIC_CAST(form, TESForm, TESObjectWEAP);
		if (weap)
		{
			g_string->Assign(PASS_COMMAND_ARGS, weap->shellCasingModel.GetModelPath());
			return true;
		}
	}
	g_string->Assign(PASS_COMMAND_ARGS, "");
	return true;
}

bool Cmd_SetWeaponShellCasingModel_Execute(COMMAND_ARGS)
{
	TESForm *form = NULL;
	char nifPath[512] = {0};

	if (ExtractArgs(EXTRACT_ARGS, &form, &nifPath) && (form = form->TryGetREFRParent()))
	{
		TESObjectWEAP *weap = DYNAMIC_CAST(form, TESForm, TESObjectWEAP);
		if (weap) weap->shellCasingModel.SetModelPath(nifPath);
	}
	return true;
}
