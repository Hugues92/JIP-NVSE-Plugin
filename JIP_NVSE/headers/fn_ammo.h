#pragma once

DEFINE_COMMAND_PLUGIN(GetAmmoTraitNumeric, "None", 0, 2, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(SetAmmoTraitNumeric, "None", 0, 3, kParams_JIP_OneForm_OneInt_OneFloat);
DEFINE_COMMAND_PLUGIN(GetAmmoProjectile, "None", 0, 1, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(SetAmmoProjectile, "None", 0, 2, kParams_JIP_OneForm_OneOptionalForm);

bool Cmd_GetAmmoTraitNumeric_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESForm *form = NULL;
	UInt32 traitID = 0;

	if (ExtractArgs(EXTRACT_ARGS, &form, &traitID) && ValidInput(traitID, 2) && (form = form->TryGetREFRParent()))
	{
		TESAmmo *ammo = DYNAMIC_CAST(form, TESForm, TESAmmo);
		if (ammo)
		{
			if (!traitID) *result = ammo->speed;
			else if (traitID == 1) *result = ammo->projPerShot;
			else *result = ammo->ammoPercentConsumed;
		}
	}
	return true;
}

bool Cmd_SetAmmoTraitNumeric_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESForm *form = NULL;
	UInt32 traitID = 0;
	float val = 0;

	if (ExtractArgs(EXTRACT_ARGS, &form, &traitID, &val) && ValidInput(traitID, 2) && (form = form->TryGetREFRParent()))
	{
		TESAmmo *ammo = DYNAMIC_CAST(form, TESForm, TESAmmo);
		if (ammo)
		{
			if (!traitID)
			{
				*result = ammo->speed;
				ammo->speed = val;
			}
			else if (traitID == 1)
			{
				*result = ammo->projPerShot;
				ammo->projPerShot = val;
			}
			else
			{
				*result = ammo->ammoPercentConsumed;
				ammo->ammoPercentConsumed = val;
			}
		}
	}
	return true;
}

bool Cmd_GetAmmoProjectile_Execute(COMMAND_ARGS)
{
	UInt32 *refResult = (UInt32*)result;
	*refResult = 0;
	TESForm *form = NULL;

	if (ExtractArgs(EXTRACT_ARGS, &form) && (form = form->TryGetREFRParent()))
	{
		TESAmmo *ammo = DYNAMIC_CAST(form, TESForm, TESAmmo);
		if (ammo && ammo->projectile) *refResult = ammo->projectile->refID;
	}
	return true;
}

bool Cmd_SetAmmoProjectile_Execute(COMMAND_ARGS)
{
	UInt32 *refResult = (UInt32*)result;
	*refResult = 0;
	TESForm *form = NULL, *projForm = NULL;

	if (ExtractArgs(EXTRACT_ARGS, &form, &projForm) && (form = form->TryGetREFRParent()))
	{
		TESAmmo *ammo = DYNAMIC_CAST(form, TESForm, TESAmmo);
		if (ammo)
		{
			BGSProjectile *proj = projForm ? DYNAMIC_CAST(projForm, TESForm, BGSProjectile) : NULL;
			if (ammo->projectile) *refResult = ammo->projectile->refID;
			ammo->projectile = proj;
		}
	}
	return true;
}
