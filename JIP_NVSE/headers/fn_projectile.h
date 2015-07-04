#pragma once

DEFINE_COMMAND_PLUGIN(GetProjectileTraitNumeric, "None", 0, 2, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(SetProjectileTraitNumeric, "None", 0, 3, kParams_JIP_OneForm_OneInt_OneFloat);
DEFINE_COMMAND_PLUGIN(GetProjectileFlag, "None", 0, 2, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(SetProjectileFlag, "None", 0, 3, kParams_JIP_OneForm_TwoInts);
DEFINE_COMMAND_PLUGIN(GetProjectileExplosion, "None", 0, 1, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(SetProjectileExplosion, "None", 0, 2, kParams_JIP_OneForm_OneOptionalForm);
DEFINE_COMMAND_PLUGIN(GetProjectileRefSource, "None", 1, 0, NULL);
DEFINE_COMMAND_PLUGIN(SetProjectileRefSource, "None", 1, 1, kParams_JIP_OneObjectRef);
DEFINE_COMMAND_PLUGIN(GetProjectileRefWeapon, "None", 1, 0, NULL);
DEFINE_COMMAND_PLUGIN(SetProjectileRefWeapon, "None", 1, 1, kParams_OneObjectID);
DEFINE_COMMAND_PLUGIN(GetProjectileRefLifeTime, "None", 1, 0, NULL);
DEFINE_COMMAND_PLUGIN(GetProjectileRefDistanceTraveled, "None", 1, 0, NULL);
DEFINE_COMMAND_PLUGIN(GetProjectileRefDamage, "None", 1, 0, NULL);
DEFINE_COMMAND_PLUGIN(SetProjectileRefDamage, "None", 1, 1, kParams_OneFloat);
DEFINE_COMMAND_PLUGIN(GetProjectileRefSpeedMult, "None", 1, 0, NULL);
DEFINE_COMMAND_PLUGIN(SetProjectileRefSpeedMult, "None", 1, 1, kParams_OneFloat);

bool Cmd_GetProjectileTraitNumeric_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESForm *form = NULL;
	UInt32 traitID = 0;

	if (ExtractArgs(EXTRACT_ARGS, &form, &traitID) && ValidInput(traitID, 14))
	{
		BGSProjectile *proj = DYNAMIC_CAST(form, TESForm, BGSProjectile);
		if (proj)
		{
			if (!traitID) *result = proj->type;
			else if (traitID == 1) *result = proj->gravity;
			else if (traitID == 2) *result = proj->speed;
			else if (traitID == 3) *result = proj->range;
			else if (traitID == 4) *result = proj->tracerChance;
			else if (traitID == 5) *result = proj->altProximity;
			else if (traitID == 6) *result = proj->altTimer;
			else if (traitID == 7) *result = proj->flashDuration;
			else if (traitID == 8) *result = proj->fadeDuration;
			else if (traitID == 9) *result = proj->impactForce;
			else if (traitID == 10) *result = proj->rotationX;
			else if (traitID == 11) *result = proj->rotationY;
			else if (traitID == 12) *result = proj->rotationZ;
			else if (traitID == 13) *result = proj->bouncyMult;
			else *result = proj->soundLevel;
		}
	}
	return true;
}

bool Cmd_SetProjectileTraitNumeric_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESForm *form = NULL;
	UInt32 traitID = 0;
	float val = 0;

	if (ExtractArgs(EXTRACT_ARGS, &form, &traitID, &val) && ValidInput(traitID, 14))
	{
		BGSProjectile *proj = DYNAMIC_CAST(form, TESForm, BGSProjectile);
		if (proj)
		{
			if (!traitID)
			{
				if ((val == 1) || (val == 2) || (val == 4) || (val == 8) || (val == 16))
				{
					*result = proj->type;
					proj->type = val;
				}
			}
			else if (traitID == 1)
			{
				*result = proj->gravity;
				proj->gravity = val;
			}
			else if (traitID == 2)
			{
				*result = proj->speed;
				proj->speed = val;
			}
			else if (traitID == 3)
			{
				*result = proj->range;
				proj->range = val;
			}
			else if (traitID == 4)
			{
				*result = proj->tracerChance;
				proj->tracerChance = val;
			}
			else if (traitID == 5)
			{	
				*result = proj->altProximity;
				proj->altProximity = val;
			}
			else if (traitID == 6)
			{	
				*result = proj->altTimer;
				proj->altTimer = val;
			}
			else if (traitID == 7)
			{	
				*result = proj->flashDuration;
				proj->flashDuration = val;
			}
			else if (traitID == 8)
			{	
				*result = proj->fadeDuration;
				proj->fadeDuration = val;
			}
			else if (traitID == 9)
			{	
				*result = proj->impactForce;
				proj->impactForce = val;
			}
			else if (traitID == 10)
			{	
				*result = proj->rotationX;
				proj->rotationX = val;
			}
			else if (traitID == 11)
			{	
				*result = proj->rotationY;
				proj->rotationY = val;
			}
			else if (traitID == 12)
			{	
				*result = proj->rotationZ;
				proj->rotationZ = val;
			}
			else if (traitID == 13)
			{
				*result = proj->bouncyMult;
				proj->bouncyMult = val;
			}
			else if ((val >= 0) && (val < 3))
			{
				*result = proj->soundLevel;
				proj->soundLevel = val;
			}
		}
	}
	return true;
}

bool Cmd_GetProjectileFlag_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESForm *form = NULL;
	UInt32 flag = 0;

	if (ExtractArgs(EXTRACT_ARGS, &form, &flag) && ValidInput(flag, 11))
	{
		BGSProjectile *proj = DYNAMIC_CAST(form, TESForm, BGSProjectile);
		if (proj) *result = (proj->projFlags & (1 << flag)) ? 1 : 0;
	}
	return true;
}

bool Cmd_SetProjectileFlag_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESForm *form = NULL;
	UInt32 flag = 0, val = 0;

	if (ExtractArgs(EXTRACT_ARGS, &form, &flag, &val) && ValidInput(flag, 11))
	{
		BGSProjectile *proj = DYNAMIC_CAST(form, TESForm, BGSProjectile);
		if (proj)
		{
			*result = (proj->projFlags & (1 << flag)) ? 1 : 0;
			proj->SetFlag(1 << flag, val != 0);
		}
	}
	return true;
}

bool Cmd_GetProjectileExplosion_Execute(COMMAND_ARGS)
{
	UInt32 *refResult = (UInt32*)result;
	*refResult = 0;
	TESForm *form = NULL;

	if (ExtractArgs(EXTRACT_ARGS, &form))
	{
		BGSProjectile *proj = DYNAMIC_CAST(form, TESForm, BGSProjectile);
		if (proj && proj->explosion) *refResult = proj->explosion->refID;
	}
	return true;
}

bool Cmd_SetProjectileExplosion_Execute(COMMAND_ARGS)
{
	UInt32 *refResult = (UInt32*)result;
	*refResult = 0;
	TESForm *projForm = NULL, *explForm = NULL;

	if (ExtractArgs(EXTRACT_ARGS, &projForm, &explForm))
	{
		BGSProjectile *proj = DYNAMIC_CAST(projForm, TESForm, BGSProjectile);
		if (proj)
		{
			BGSExplosion *expl = explForm ? DYNAMIC_CAST(explForm, TESForm, BGSExplosion) : NULL;
			if (proj->explosion) *refResult = proj->explosion->refID;
			proj->explosion = expl;
		}
	}
	return true;
}

bool ProjectileRefGet_Execute(COMMAND_ARGS, UInt8 which)
{
	*result = 0;

	if (thisObj)
	{
		Projectile *proj = DYNAMIC_CAST(thisObj, TESObjectREFR, Projectile);
		if (proj)
		{
			if (!which)
			{
				if (proj->sourceRef) *(UInt32*)result = proj->sourceRef->refID;
			}
			else if (which == 1)
			{
				if (proj->sourceWeap) *(UInt32*)result = proj->sourceWeap->refID;
			}
			else if (which == 2) *result = proj->elapsedTime;
			else if (which == 3) *result = proj->distTravelled;
			else if (which == 4) *result = proj->hitDamage;
			else *result = proj->speedMult2;
		}
	}
	return true;
}

bool Cmd_GetProjectileRefSource_Execute(COMMAND_ARGS)
{
	return ProjectileRefGet_Execute(PASS_COMMAND_ARGS, 0);
}

bool Cmd_GetProjectileRefWeapon_Execute(COMMAND_ARGS)
{
	return ProjectileRefGet_Execute(PASS_COMMAND_ARGS, 1);
}

bool Cmd_GetProjectileRefLifeTime_Execute(COMMAND_ARGS)
{
	return ProjectileRefGet_Execute(PASS_COMMAND_ARGS, 2);
}

bool Cmd_GetProjectileRefDistanceTraveled_Execute(COMMAND_ARGS)
{
	return ProjectileRefGet_Execute(PASS_COMMAND_ARGS, 3);
}

bool Cmd_GetProjectileRefDamage_Execute(COMMAND_ARGS)
{
	return ProjectileRefGet_Execute(PASS_COMMAND_ARGS, 4);
}

bool Cmd_GetProjectileRefSpeedMult_Execute(COMMAND_ARGS)
{
	return ProjectileRefGet_Execute(PASS_COMMAND_ARGS, 5);
}

bool Cmd_SetProjectileRefSource_Execute(COMMAND_ARGS)
{
	UInt32 *refResult = (UInt32*)result;
	*refResult = 0;
	TESObjectREFR *newSource = NULL;

	if (thisObj && ExtractArgs(EXTRACT_ARGS, &newSource))
	{
		Projectile *proj = DYNAMIC_CAST(thisObj, TESObjectREFR, Projectile);
		if (proj)
		{
			if (proj->sourceRef) *refResult = proj->sourceRef->refID;
			proj->sourceRef = newSource;
		}
	}
	return true;
}

bool Cmd_SetProjectileRefWeapon_Execute(COMMAND_ARGS)
{
	UInt32 *refResult = (UInt32*)result;
	*refResult = 0;
	TESForm *form = NULL;

	if (thisObj && ExtractArgs(EXTRACT_ARGS, &form))
	{
		Projectile *proj = DYNAMIC_CAST(thisObj, TESObjectREFR, Projectile);
		TESObjectWEAP *weap = DYNAMIC_CAST(form, TESForm, TESObjectWEAP);
		if (proj && weap)
		{
			if (proj->sourceWeap) *refResult = proj->sourceWeap->refID;
			proj->sourceWeap = weap;
		}
	}
	return true;
}

bool Cmd_SetProjectileRefDamage_Execute(COMMAND_ARGS)
{
	*result = 0;
	float dmg = 0;

	if (thisObj && ExtractArgs(EXTRACT_ARGS, &dmg))
	{
		Projectile *proj = DYNAMIC_CAST(thisObj, TESObjectREFR, Projectile);
		if (proj)
		{
			*result = proj->hitDamage;
			proj->hitDamage = dmg;
		}
	}
	return true;
}

bool Cmd_SetProjectileRefSpeedMult_Execute(COMMAND_ARGS)
{
	*result = 0;
	float speedMult = 0;

	if (thisObj && ExtractArgs(EXTRACT_ARGS, &speedMult))
	{
		Projectile *proj = DYNAMIC_CAST(thisObj, TESObjectREFR, Projectile);
		if (proj)
		{
			*result = proj->speedMult2;
			proj->speedMult2 = speedMult;
		}
	}
	return true;
}
