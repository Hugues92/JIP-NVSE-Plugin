#pragma once

DEFINE_COMMAND_PLUGIN(GetExplosionTraitNumeric, "None", 0, 2, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(SetExplosionTraitNumeric, "None", 0, 3, kParams_JIP_OneForm_OneInt_OneFloat);
DEFINE_COMMAND_PLUGIN(GetExplosionTraitForm, "None", 0, 2, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(SetExplosionTraitForm, "None", 0, 3, kParams_JIP_OneForm_OneInt_OneOptionalForm);
DEFINE_COMMAND_PLUGIN(GetExplosionFlag, "None", 0, 2, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(SetExplosionFlag, "None", 0, 3, kParams_JIP_OneForm_TwoInts);

bool Cmd_GetExplosionTraitNumeric_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESForm *form = NULL;
	UInt32 traitID = 0;

	if (ExtractArgs(EXTRACT_ARGS, &form, &traitID) && ValidInput(traitID, 6))
	{
		BGSExplosion *explosion = DYNAMIC_CAST(form, TESForm, BGSExplosion);
		if (explosion)
		{
			if (!traitID) *result = explosion->force;
			else if (traitID == 1) *result = explosion->damage;
			else if (traitID == 2) *result = explosion->radius;
			else if (traitID == 3) *result = explosion->ISradius;
			else if (traitID == 4) *result = explosion->RADlevel;
			else if (traitID == 5) *result = explosion->dissipationTime;
			else *result = explosion->RADradius;
		}
	}
	return true;
}

bool Cmd_SetExplosionTraitNumeric_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESForm *form = NULL;
	UInt32 traitID = 0;
	float val = 0;

	if (ExtractArgs(EXTRACT_ARGS, &form, &traitID, &val) && ValidInput(traitID, 6))
	{
		BGSExplosion *explosion = DYNAMIC_CAST(form, TESForm, BGSExplosion);
		if (explosion)
		{
			if (!traitID)
			{
				*result = explosion->force;
				explosion->force = val;
			}
			else if (traitID == 1)
			{	
				*result = explosion->damage;
				explosion->damage = val;
			}
			else if (traitID == 2)
			{	
				*result = explosion->radius;
				explosion->radius = val;
			}
			else if (traitID == 3)
			{	
				*result = explosion->ISradius;
				explosion->ISradius = val;
			}
			else if (traitID == 4)
			{	
				*result = explosion->RADlevel;
				explosion->RADlevel = val;
			}
			else if (traitID == 5)
			{	
				*result = explosion->dissipationTime;
				explosion->dissipationTime = val;
			}
			else
			{	
				*result = explosion->RADradius;
				explosion->RADradius = val;
			}
		}
	}
	return true;
}

bool Cmd_GetExplosionTraitForm_Execute(COMMAND_ARGS)
{
	UInt32 *refResult = (UInt32*)result, traitID = 0;
	*refResult = 0;
	TESForm *form = NULL;

	if (ExtractArgs(EXTRACT_ARGS, &form, &traitID) && ValidInput(traitID, 4))
	{
		BGSExplosion *explosion = DYNAMIC_CAST(form, TESForm, BGSExplosion);
		if (explosion)
		{
			if (!traitID)
			{
				if (explosion->light) *refResult = explosion->light->refID;
			}
			else if (traitID == 1)
			{
				if (explosion->sound1) *refResult = explosion->sound1->refID;
			}
			else if (traitID == 2)
			{
				if (explosion->sound2) *refResult = explosion->sound2->refID;
			}
			else if (traitID == 3)
			{
				if (explosion->impactDataSet) *refResult = explosion->impactDataSet->refID;
			}
			else if (explosion->placedObj) *refResult = explosion->placedObj->refID;
		}
	}
	return true;
}

bool Cmd_SetExplosionTraitForm_Execute(COMMAND_ARGS)
{
	UInt32 *refResult = (UInt32*)result, traitID = 0;
	*refResult = 0;
	TESForm *form = NULL, *object = NULL;

	if (ExtractArgs(EXTRACT_ARGS, &form, &traitID, &object) && ValidInput(traitID, 4))
	{
		BGSExplosion *explosion = DYNAMIC_CAST(form, TESForm, BGSExplosion);
		if (explosion)
		{
			if (!traitID)
			{
				TESObjectLIGH *light = object ? DYNAMIC_CAST(object, TESForm, TESObjectLIGH) : NULL;
				if (explosion->light) *refResult = explosion->light->refID;
				explosion->light = light;
			}
			else if (traitID < 3)
			{
				TESSound *sound = object ? DYNAMIC_CAST(object, TESForm, TESSound) : NULL;
				if (traitID == 1)
				{
					if (explosion->sound1) *refResult = explosion->sound1->refID;
					explosion->sound1 = sound;
				}
				else
				{
					if (explosion->sound2) *refResult = explosion->sound2->refID;
					explosion->sound2 = sound;
				}
			}
			else if (traitID == 3)
			{
				BGSImpactDataSet *impactDS = object ? DYNAMIC_CAST(object, TESForm, BGSImpactDataSet) : NULL;
				if (explosion->impactDataSet) *refResult = explosion->impactDataSet->refID;
				explosion->impactDataSet = impactDS;
			}
			else
			{
				if (explosion->placedObj) *refResult = explosion->placedObj->refID;
				explosion->placedObj = object;
			}
		}
	}
	return true;
}

bool Cmd_GetExplosionFlag_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESForm *form = NULL;
	UInt32 flag = 0;

	if (ExtractArgs(EXTRACT_ARGS, &form, &flag) && ValidInput(flag, 6, 1))
	{
		BGSExplosion *explosion = DYNAMIC_CAST(form, TESForm, BGSExplosion);
		if (explosion) *result = (explosion->explFlags & (1 << flag)) ? 1 : 0;
	}
	return true;
}

bool Cmd_SetExplosionFlag_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESForm *form = NULL;
	UInt32 flag = 0, val = 0;

	if (ExtractArgs(EXTRACT_ARGS, &form, &flag, &val) && ValidInput(flag, 6, 1))
	{
		BGSExplosion *explosion = DYNAMIC_CAST(form, TESForm, BGSExplosion);
		if (explosion)
		{
			*result = (explosion->explFlags & (1 << flag)) ? 1 : 0;
			explosion->SetFlag(1 << flag, val != 0);
		}
	}
	return true;
}
